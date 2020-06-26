#include "FBSession.h"
#include <winrt/Windows.Security.Cryptography.DataProtection.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Security.Credentials.h>

#undef GetObject
#include "FBPaginatedArray.h"
#include "FBSingleValue.h"
#include <FBConstants.h>
#include "SDKMessage.h"

#include <sstream>
#include <pplawait.h>

using namespace std;
using namespace winrt;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Security::Authentication::Web;
#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
using namespace Windows::Security::Authentication::Web::Core;
#endif
using namespace Windows::Security::Credentials;
using namespace Windows::Security::Cryptography;
using namespace Windows::Security::Cryptography::DataProtection;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;


#define INT64_STRING_BUFSIZE 65
extern const wchar_t* ErrorObjectJson;

#define FBAccountProvider L"https://www.facebook.com"

#define TICKS_PER_SECOND    10000000
#define SECONDS_PER_MINUTE  60
#define _90_MINUTES_IN_TICKS (90LL * SECONDS_PER_MINUTE * TICKS_PER_SECOND)

#define ScopeKey        L"scope"
#define DisplayKey      L"display"
#define ResponseTypeKey L"response_type"
#define DefaultScope    L"public_profile,email,user_friends"
#define DefaultDisplay  L"popup"
#define DefaultResponse L"token"
#define AuthTypeKey     L"auth_type"
#define Rerequest       L"rerequest"
#define	Reauthorize     L"reauthorize"
#define RedirectUriKey  L"redirect_uri"

#define SDK_APP_DATA_CONTAINER L"winsdkfb" // TODO: Should we move this?
#define GRANTED_PERMISSIONS_KEY L"granted_permissions"

namespace winsdkfb
{
	HANDLE login_evt = nullptr;

	FBSession::FBSession()
		:  _loggedIn(false)
	{
		if (!::winsdkfb::login_evt)
		{
			::winsdkfb::login_evt = CreateEventEx(nullptr, nullptr, 0, DELETE | SYNCHRONIZE);
		}
		_APIMajorVersion = 2;
		_APIMinorVersion = 6;
		_webViewRedirectDomain = FACEBOOK_DESKTOP_SERVER_NAME;
		_webViewRedirectPath = FACEBOOK_LOGIN_SUCCESS_PATH;
	}

	FBSession::~FBSession()
	{
		if (::winsdkfb::login_evt)
		{
			CloseHandle(::winsdkfb::login_evt);
			::winsdkfb::login_evt = nullptr;
		}
	}


	wstring FBSession::FBAppId() const {
		if (_FBAppId.empty())
		{
			throw hresult_invalid_argument(SDKMessageMissingAppID);
		}
		return _FBAppId;
	}

	void FBSession::FBAppId(wstring const& value)
	{
		_FBAppId = value;
	}

	wstring FBSession::WinAppId() const {
		return _WinAppId;
	}

	void FBSession::WinAppId(wstring const& value)
	{
		_WinAppId = value;
	}

	wstring FBSession::AppResponse() const {
		return _AppResponse;
	}

	bool FBSession::LoggedIn() const {
		return _loggedIn;
	}

	winsdkfb::FBAccessTokenData FBSession::AccessTokenData() const {
		return _AccessTokenData;
	}

	void FBSession::AccessTokenData(winsdkfb::FBAccessTokenData value)
	{
		_AccessTokenData = value;

		// If token have been updated, make sure to save updated token
		TrySaveTokenData();
	}

	int32_t FBSession::APIMajorVersion() const {
		return _APIMajorVersion;
	}

	int32_t FBSession::APIMinorVersion() const {
		return _APIMinorVersion;
	}

	winsdkfb::Graph::FBUser FBSession::User() const {
		return _user;
	}

	wstring FBSession::WebViewRedirectDomain() const {
		return _webViewRedirectDomain;
	}

	wstring FBSession::WebViewRedirectPath() const {
		return _webViewRedirectPath;
	}

	IAsyncAction FBSession::LogoutAsync()
	{
		_user = winsdkfb::Graph::FBUser();
		_FBAppId.clear();
		_WinAppId.clear();
		_AccessTokenData = FBAccessTokenData();
		_AppResponse.clear();
		_loggedIn = false;

		FBDialog::DeleteCookies();

		return TryDeleteTokenDataAsync();
	}

	concurrency::task<winsdkfb::FBResult> FBSession::ShowFeedDialogAsync(unordered_map<wstring, wstring> const parameters) const {
		winsdkfb::FBResult result;
		auto dialog = winrt::make<FBDialog>().as<FBDialog>();
		try {
			result = co_await dialog->ShowFeedDialogAsync(parameters);
		}
		catch (hresult_error e) {
			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			result = FBResult(err);
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::ShowRequestsDialogAsync(unordered_map<wstring, wstring> const parameters) const {
		winsdkfb::FBResult result;
		auto dialog = winrt::make<FBDialog>().as<FBDialog>();
		try {
			result = co_await dialog->ShowRequestsDialogAsync(parameters);
		}
		catch (hresult_error e) {
			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			result = FBResult(err);
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::ShowSendDialogAsync(unordered_map<wstring, wstring> const parameters) const {
		winsdkfb::FBResult result;
		auto dialog = winrt::make<FBDialog>().as<FBDialog>();
		try {
			result = co_await dialog->ShowSendDialogAsync(parameters);
		}
		catch (hresult_error e) {
			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			result = FBResult(err);
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::LoginAsync()
	{
		return LoginAsync(FBPermissions(), winsdkfb::SessionLoginBehavior::DefaultOrdering);
	}

	concurrency::task<winsdkfb::FBResult> FBSession::LoginAsync(winsdkfb::FBPermissions permissions)
	{
		return LoginAsync(permissions, winsdkfb::SessionLoginBehavior::DefaultOrdering);
	}

	concurrency::task<winsdkfb::FBResult> FBSession::LoginAsync(winsdkfb::FBPermissions permissions, winsdkfb::SessionLoginBehavior behavior)
	{
		unordered_map<wstring, wstring> parameters;
		parameters[ScopeKey] = permissions.ToString();

		if (LoggedIn()) {
			parameters[AuthTypeKey] = Rerequest;
		}
		switch (behavior)
		{
		case SessionLoginBehavior::WebView:
			_asyncResult = co_await TryLoginViaWebViewAsync(parameters);
			break;

		case SessionLoginBehavior::WebAuth:
			_asyncResult = co_await TryLoginViaWebAuthBrokerAsync(parameters);
			break;

		case SessionLoginBehavior::WebAccountProvider:
			_asyncResult = co_await TryLoginViaWebAccountProviderAsync(permissions);
			break;

		case SessionLoginBehavior::DefaultOrdering:
			_asyncResult = co_await TryLoginViaWebAccountProviderAsync(permissions);

			if (!_asyncResult.Succeeded() || (_asyncResult.ErrorInfo().Code() == static_cast<int>(ErrorCode::ErrorCodeWebAccountProviderNotFound))) {
				_asyncResult = co_await TryLoginViaWebViewAsync(parameters);
				if (!_asyncResult.Succeeded()) {
					_asyncResult = co_await TryLoginViaWebAuthBrokerAsync(parameters);
				}
			}
			break;

		case SessionLoginBehavior::Silent:
			_asyncResult = co_await TryLoginSilentlyAsync(parameters);
			break;
		default:
			OutputDebugString(L"Invalid SessionLoginBehavior member!\n");
			// TODO need a real error code
			_asyncResult = FBResult(FBError(0, L"Login Error", L"Invalid SessionLoginBehavior member"));
			break;
		}

		auto userInfoResult = co_await TryGetUserInfoAfterLoginAsync(_asyncResult);
		auto finalResult = co_await TryGetAppPermissionsAfterLoginAsync(userInfoResult);

		if (!finalResult.Succeeded()) {
			_loggedIn = false;
			AccessTokenData(FBAccessTokenData());

			finalResult = FBResult(FBError(0, L"Unexpected error", L"Log in attempt failed"));
			OutputDebugString(L"LoginAsync was about to return nullptr, created FBResult object to return instead");
		}
		else {
			SaveGrantedPermissions();
		}
		co_return finalResult;
	}

	task<FBResult> FBSession::ReauthorizeAsync(FBPermissions permissions)
	{		
		unordered_map<wstring, wstring> parameters;
		parameters[ScopeKey] = permissions.ToString();

		if (LoggedIn())
		{
			parameters[AuthTypeKey] = Reauthorize;
		}

		auto graphResult = co_await TryLoginViaWebViewAsync(parameters);
		auto userInfoResult = co_await TryGetUserInfoAfterLoginAsync(graphResult);
		auto finalResult = co_await TryGetAppPermissionsAfterLoginAsync(userInfoResult);
		if (finalResult.Succeeded()) {
			SaveGrantedPermissions();
		}
		co_return finalResult;
	}

	void FBSession::SetApiVersion(int32_t major, int32_t minor)
	{
		_APIMajorVersion = major;
		_APIMinorVersion = minor;
	}

	void FBSession::SetWebViewRedirectUrl(wstring const& domain, wstring const& path)
	{
		if (!domain.empty()) {
			_webViewRedirectDomain = domain.c_str();
		}
		if (!path.empty()) {
			_webViewRedirectPath = path.c_str();
		}
	}

	shared_ptr<FBSession> FBSession::ActiveSession()
	{
		static shared_ptr<FBSession> activeFBSession = make_shared<FBSession>();
		return activeFBSession;
	}

	ApplicationDataContainer FBSession::DataContainer()
	{
		const auto localSettings = ApplicationData::Current().LocalSettings();
		if (!localSettings.Containers().HasKey(SDK_APP_DATA_CONTAINER))
		{
			return localSettings.CreateContainer(SDK_APP_DATA_CONTAINER, ApplicationDataCreateDisposition::Always);
		}
		return localSettings.Containers().Lookup(SDK_APP_DATA_CONTAINER);
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryRefreshAccessTokenAsync() const {
		WebTokenRequestResult result{ nullptr };

		const auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
		if (provider) {
			const auto perms = GetGrantedPermissions();
			const WebTokenRequest request(provider, perms, FBAppId());
			request.Properties().Insert(RedirectUriKey, GetWebAccountProviderRedirectUriString());
			result = co_await WebAuthenticationCoreManager::GetTokenSilentlyAsync(request);
			if (result) {
				co_await result.InvalidateCacheAsync();

			}
		}

		co_return FBResultFromTokenRequestResult(result);
	}

	Uri FBSession::BuildLoginUri(unordered_map<wstring, wstring> parameters) const {
		auto session = FBSession::ActiveSession();
		hstring apiVersion(L"");
		if (APIMajorVersion())
		{
			wstringstream apiStream;
			apiStream << L"v" + APIMajorVersion() << L"." << APIMinorVersion() << L"/";
			apiVersion = apiStream.str().c_str();
		}
		wstringstream uriStream;
		uriStream << L"https://www.facebook.com/" << apiVersion.c_str() << L"dialog/oauth?client_id=" << session->FBAppId().c_str();

		// Use some reasonable default login parameters
		hstring scope(DefaultScope);
		hstring displayType(DefaultDisplay);
		hstring responseType(DefaultResponse);

		uriStream << L"&" << RedirectUriKey << L"=" << GetWebAuthRedirectUriString().c_str();

		for (auto const& parameter : parameters) {
			auto key = parameter.first;
			auto value = parameter.second;
			if (!value.empty()) {
				if (compare_ordinal(key.c_str(), ScopeKey) == 0) {
					scope = value;
				}
				else if (compare_ordinal(key.c_str(), DisplayKey) == 0) {
					displayType = value;
				}
				else if (compare_ordinal(key.c_str(), ResponseTypeKey) == 0) {
					responseType = value;
				}
				else {
					uriStream << "&" << key.c_str() << "=" << value.c_str();
				}
			}
		}

		uriStream << "&" << ScopeKey << "=" << scope.c_str()
			<< "&" << DisplayKey << "=" << displayType.c_str()
			<< "&" << ResponseTypeKey << "=" << responseType.c_str();

		return Uri(uriStream.str().c_str());
	}

	wstring FBSession::GetWebAuthRedirectUriString() {
		return WebAuthenticationBroker::GetCurrentApplicationCallbackUri().DisplayUri().c_str();
	}

	concurrency::task<winsdkfb::FBResult> FBSession::GetUserInfoAsync(
		winsdkfb::FBAccessTokenData const tokenData
	) const {
		unordered_map<wstring, wstring> parameters;
		parameters[L"fields"] = L"gender,link,first_name,last_name,locale,timezone,email,updated_time,verified,name,id,picture";
		auto objectFactory = JsonClassFactory([](wstring jsonText) -> FBResult
		{
			auto user = Graph::FBUser::FromJson(jsonText);
			return FBResult(std::any_cast<Graph::FBUser>(user));
		});

		auto value = FBSingleValue(
			L"/me",
			parameters,
			objectFactory);

		auto result = co_await value.GetAsync();
		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::CheckForExistingTokenAsync() const {
		winsdkfb::FBResult result;
		if (LoggedIn())
		{
			result = FBResult(AccessTokenData());
		}
		else
		{
			try {
				auto folder = ApplicationData::Current().LocalFolder();
				auto item = co_await folder.TryGetItemAsync(L"FBSDKData");
				if (item != nullptr) {
					auto file = item.as<StorageFile>();
					auto protectedBuffer = co_await FileIO::ReadBufferAsync(file);
					DataProtectionProvider provider;
					auto clearBuffer = co_await provider.UnprotectAsync(protectedBuffer);
					auto clearText = CryptographicBuffer::ConvertBinaryToString(BinaryStringEncoding::Utf16LE, clearBuffer);

					wstring values(clearText.c_str());
					size_t pos = values.find(L",");

					if (pos != wstring::npos)
					{
						std::time_t expirationTime;
						std::time_t dataAccessExpirationTime = 0;
						wstring dataAccessExpirationString;
						wstring accessToken(values.substr(0, pos).c_str());
						wstring msg(L"Access Token: " + accessToken + L"\n");
						OutputDebugString(msg.c_str());

						size_t nextPos = values.find(L",", pos + 1);
						wstring expirationString(values.substr(pos + 1, nextPos - (pos + 1)).c_str());
						msg = L"Expiration: " + expirationString + L"\n";
						OutputDebugString(msg.c_str());

						if (nextPos != wstring::npos)
						{
							dataAccessExpirationString = hstring(values.substr(nextPos + 1, wstring::npos).c_str());
						}
						FBAccessTokenData cachedData;
						if (expirationString != L"0") {
							expirationTime =  _wtoi64(expirationString.data());
							if (dataAccessExpirationString != L"0") {
								dataAccessExpirationTime= _wtoi64(dataAccessExpirationString.data());
							}
							cachedData = winsdkfb::FBAccessTokenData(
								accessToken, 
								winrt::clock::from_time_t(expirationTime), 
								winrt::clock::from_time_t(dataAccessExpirationTime)
							);
						}
						result = FBResult(cachedData);
					}
				}
			}
			catch (...) {
#ifdef _DEBUG
				OutputDebugString(L"Couldn't decrypt cached token.  Continuing without cached token data.\n");
#endif
			}
		}
		co_return result;
	}

	fire_and_forget FBSession::TrySaveTokenData() const {
		if (LoggedIn())
		{
			wchar_t buffer[INT64_STRING_BUFSIZE];
			_i64tow_s(
				WindowsTickToUnixSeconds(_AccessTokenData.ExpirationDate().time_since_epoch().count()),
				buffer, INT64_STRING_BUFSIZE, 10);
			wstringstream tokenStream;
			
			if (_AccessTokenData.HasDataAccessExpirationDate()) {
				wchar_t daBuffer[INT64_STRING_BUFSIZE];
				_i64tow_s(
					WindowsTickToUnixSeconds(_AccessTokenData.DataAccessExpirationDate().time_since_epoch().count()),
					daBuffer, INT64_STRING_BUFSIZE, 10);
				tokenStream << AccessTokenData().AccessToken().c_str()
							<< L"," << hstring(buffer).c_str()
							<< L"," << hstring(daBuffer).c_str();
			} else {
				tokenStream << AccessTokenData().AccessToken().c_str() 
							<< L"," << hstring(buffer).c_str() 
							<< L",0";
			}

			const hstring tokenData(tokenStream.str().c_str());
			const auto dataBuff = CryptographicBuffer::ConvertStringToBinary(tokenData, BinaryStringEncoding::Utf16LE);

			const DataProtectionProvider provider(L"LOCAL=user");
			const auto protectedData = co_await provider.ProtectAsync(dataBuff);
			const auto folder = ApplicationData::Current().LocalFolder();
			const auto file = co_await folder.CreateFileAsync(L"FBSDKData", CreationCollisionOption::OpenIfExists);
			try {
				co_await FileIO::WriteBufferAsync(file, protectedData);
			} catch (...) {
			
			}
		}
	}

	IAsyncAction FBSession::TryDeleteTokenDataAsync() {
		const auto folder = ApplicationData::Current().LocalFolder();
#ifdef _DEBUG
		wstringstream msgStream;
		msgStream << L"Deleting cached token from " << folder.Path().c_str() << L"\n";
		const hstring msg(msgStream.str().c_str());
		OutputDebugString(msg.c_str());
#endif
		try {
			const auto item = co_await folder.TryGetItemAsync(L"FBSDKData");
			item.DeleteAsync();
		}
		catch (...) {
			//Do nothing here, trying to delete the cache file is a "fire and
			//forget" operation.  If it fails, we'll pick up bad token data at
			//next login, fail the login and retry, then attempt to cache new
			//valid token data.
#ifdef _DEBUG
			OutputDebugString(L"Deleting cached token file failed!\n");
#endif
		}
	}

	concurrency::task<winsdkfb::FBResult> FBSession::GetAppPermissionsAsync()
	{
		wstringstream permStream;
		permStream << L"/" << _user.Id().c_str() << L"/permissions";

		auto factory = JsonClassFactory([](wstring const& jsonText) -> FBResult
		{
			return Graph::FBPermission::FromJson(jsonText);
		});

		FBPaginatedArray permArr(
			permStream.str().c_str(),
			{},
			factory);

		auto result = co_await permArr.FirstAsync();
		if (result.Succeeded()) {
			auto perms = result.Object<std::vector<FBResult>>().value();
			std::vector<Graph::FBPermission> permissions;
			for (FBResult & perm: perms)
			{
				permissions.push_back(perm.Object<Graph::FBPermission>().value());
			}
			_AccessTokenData.SetPermissions(permissions);
		}
		co_return FBResult(_user);
	}

	winsdkfb::FBResult FBSession::ProcessAuthResult(WebAuthenticationResult authResult) const {
		winsdkfb::FBResult result;
		wstring uriString;
		winsdkfb::FBAccessTokenData tokenData;
		Uri uri{ nullptr };

		switch (authResult.ResponseStatus())
		{
		case WebAuthenticationStatus::ErrorHttp:
			//TODO: need a real error code
			result = FBResult(FBError(0,
				L"Communication error",
				L"An Http error occurred"));
			break;
		case WebAuthenticationStatus::Success:
			//TODO: need a real error code
			uriString = authResult.ResponseData();
			uri = Uri(uriString);
			tokenData = std::any_cast<FBAccessTokenData>(FBAccessTokenData::FromUri(uri));
			if (tokenData.AccessToken().empty())
			{
				result = FBResult(FBError::FromUri(uri));
			}
			else
			{
				result = FBResult(tokenData);
			}
			break;
		case WebAuthenticationStatus::UserCancel:
			result = FBResult(FBError(0,
				L"User canceled",
				L"The login operation was canceled"));
			break;
		default:
			break;
		}
		return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryGetUserInfoAfterLoginAsync(winsdkfb::FBResult loginResult)
	{
		winsdkfb::FBResult result;

		if (loginResult.Succeeded())
		{
			auto tokenData = loginResult.Object<FBAccessTokenData>();
			_AccessTokenData = tokenData.value();
			_loggedIn = true;
			TrySaveTokenData();
			result = co_await GetUserInfoAsync(_AccessTokenData);
		}
		else
		{
			result = loginResult;
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryGetAppPermissionsAfterLoginAsync(winsdkfb::FBResult loginResult)
	{
		winsdkfb::FBResult result;
		if (loginResult.Succeeded())
		{
			_user = loginResult.Object<Graph::FBUser>().value();
			result = co_await GetAppPermissionsAsync();
		}
		else
		{
			result = loginResult;
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::RunOAuthOnUiThreadAsync(unordered_map<wstring, wstring> const Parameters)
	{
		try
		{
			_asyncResult = FBResult();
			auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
			auto function = [](FBSession* self, HANDLE event, unordered_map<wstring, wstring> parameters) -> IAsyncAction {
				const auto authResult = co_await WebAuthenticationBroker::AuthenticateAsync(
					WebAuthenticationOptions::None,
					self->BuildLoginUri(parameters),
					Uri(self->GetWebAuthRedirectUriString()));
				self->_asyncResult = self->ProcessAuthResult(authResult);

				SetEvent(event);
			};

			CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(
				CoreDispatcherPriority::Normal,
				std::bind(function, this, asyncEvent, Parameters));

			co_await resume_on_signal(asyncEvent);
		}
		catch (hresult_error ex)
		{
			throw hresult_invalid_argument(SDKMessageLoginFailed);
		}
		co_return _asyncResult;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::RunWebViewLoginOnUIThreadAsync(unordered_map<wstring, wstring> const Parameters)
	{
		try
		{
			_asyncResult = FBResult();
			auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
			auto function = [](FBSession* self, HANDLE event, unordered_map<wstring, wstring> parameters) -> IAsyncAction {
				const auto result = co_await self->ShowLoginDialogAsync(parameters);
				self->_asyncResult = result;

				SetEvent(event);
			};

			CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(
				CoreDispatcherPriority::Normal,
				std::bind(function, this, asyncEvent, Parameters));

			co_await resume_on_signal(asyncEvent);
		}
		catch (hresult_error ex)
		{
			throw hresult_invalid_argument(SDKMessageLoginFailed);
		}
		co_return _asyncResult;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::ShowLoginDialogAsync(unordered_map<wstring, wstring> const Parameters)
	{
		FBResult result;
		try {
			auto dialog = winrt::make<FBDialog>().as<FBDialog>();
			result = co_await dialog->ShowLoginDialogAsync(Parameters);
		}
		catch (hresult_error e) {
			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			result = FBResult(err);
		}

		if (result.Succeeded())
		{
			auto tokenData = result.Object<FBAccessTokenData>();
			AccessTokenData(tokenData.value());
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryLoginViaWebViewAsync(
		unordered_map<wstring, wstring> const parameters
	) {
		FBResult loginResult;
		auto session = FBSession::ActiveSession();

		if (!IsRerequest(parameters)) {
			FBResult oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult.Succeeded()) {
				auto tokenData = oauthResult.Object<FBAccessTokenData>();
				if (tokenData.has_value() && !tokenData.value().IsExpired()) {
					loginResult = FBResult(tokenData.value());
				}
			}
			if (!loginResult.Succeeded()) {
				loginResult = co_await RunWebViewLoginOnUIThreadAsync(parameters);
			}
		}
		co_return loginResult;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryLoginViaWebAuthBrokerAsync(
		unordered_map<wstring, wstring> const parameters
	) {
		FBResult loginResult;
		auto session = FBSession::ActiveSession();

		if (!IsRerequest(parameters)) {
			FBResult oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult.Succeeded()) {
				auto tokenData = oauthResult.Object<FBAccessTokenData>();
				if (tokenData.has_value() && !tokenData.value().IsExpired()) {
					loginResult = FBResult(tokenData.value());
				}
			}
			else {
				loginResult = co_await RunOAuthOnUiThreadAsync(parameters);
			}
		}
		co_return loginResult;
	}

	task<FBResult> FBSession::TryLoginSilentlyAsync(
		unordered_map<wstring, wstring> parameters
	) const {
		FBResult loginResult;
		auto session = FBSession::ActiveSession();

		auto grantedPermissions = FBPermissions::FromString(GetGrantedPermissions());
		auto requestingPermissions = FBPermissions::FromString(parameters[L"scope"]);

		OutputDebugString(hstring(L"Granted permissions: " + grantedPermissions.ToString() + L"\n").c_str());
		OutputDebugString(hstring(L"Requested permissions: " + requestingPermissions.ToString() + L"\n").c_str());

		auto diffPermissions = FBPermissions::Difference(requestingPermissions, grantedPermissions);

		FBResult oauthResult;
		if (diffPermissions.Values().size() == 0) {
			oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult.Succeeded()) {
				auto tokenData = oauthResult.Object<FBAccessTokenData>();
				if (tokenData && !tokenData->IsExpired()) {
					loginResult = FBResult(tokenData);
				}
				else {
					loginResult = FBResult(FBError(0, L"Restore Session Error", L"Could not find a valid access token or token has expired"));
				}
			}
			else {
				loginResult = FBResult(FBError(0, L"Restore Session Error", L"Could not find a valid access token"));
			}
		}
		else {
			loginResult = FBResult(FBError(0, L"Restore Session Error", L"Requested permissions are different from granted"));
		}
		co_return loginResult;
	}

	void FBSession::SaveGrantedPermissions() const {
		const auto values = FBSession::DataContainer().Values();
		const auto grantedPermissions = AccessTokenData().GrantedPermissions().ToString();
		OutputDebugString(hstring(L"Granted permissions: " + grantedPermissions + L"\n").c_str());
		values.Insert(GRANTED_PERMISSIONS_KEY, box_value(grantedPermissions));
	}

	wstring FBSession::GetGrantedPermissions() const {
		const auto values = FBSession::DataContainer().Values();
		if (!values.HasKey(GRANTED_PERMISSIONS_KEY)) {
			return L"";
		}
		return unbox_value<hstring>(values.Lookup(GRANTED_PERMISSIONS_KEY)).c_str();
	}

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	wstring FBSession::GetWebAccountProviderRedirectUriString() {
		const auto package = Package::Current();
		auto packageId = package.Id();
		const wstring phoneAppId = L"s-1-15-2-993894519-2096754445-998123955-3150536948-1224592709-2849448323-2973583169";
		return L"msft-" + phoneAppId + L"://login_success";
	}

	concurrency::task<winsdkfb::FBResult> FBSession::CheckWebAccountProviderForExistingTokenAsync(
		winsdkfb::FBPermissions permissions
	) {
		winsdkfb::FBResult result;
		if (LoggedIn())
		{
			co_return FBResult(AccessTokenData());
		}
		else
		{
			auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
			if (provider != nullptr) {
				WebTokenRequest request(provider, permissions.ToString(), FBAppId());
				request.Properties().Insert(RedirectUriKey, GetWebAccountProviderRedirectUriString());
				auto requestResult = co_await WebAuthenticationCoreManager::GetTokenSilentlyAsync(request);

				if (requestResult && requestResult.ResponseStatus() == WebTokenRequestStatus::UserInteractionRequired)
				{
					result = co_await CallWebAccountProviderOnUiThreadAsync(permissions);
				}
				else {
					result = FBResultFromTokenRequestResult(requestResult);
				}
			}
		}

		co_return result;
	}

	concurrency::task<winsdkfb::FBResult> FBSession::TryLoginViaWebAccountProviderAsync(
		winsdkfb::FBPermissions permissions
	) {
		return CheckWebAccountProviderForExistingTokenAsync(permissions);
	}

	concurrency::task<winsdkfb::FBResult> FBSession::CallWebAccountProviderOnUiThreadAsync(
		winsdkfb::FBPermissions permissions
	) {
		_asyncResult = FBResult();
		auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
		auto function = [](FBSession *self, HANDLE event, winsdkfb::FBPermissions const& permissions) -> IAsyncAction {
			try {
				const auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
				WebTokenRequestResult result = nullptr;
				if (provider) {
					const WebTokenRequest request(provider, permissions.ToString(), self->FBAppId());
					request.Properties().Insert(RedirectUriKey, self->GetWebAccountProviderRedirectUriString());
					result = co_await WebAuthenticationCoreManager::RequestTokenAsync(request);
				}
				self->_asyncResult = self->FBResultFromTokenRequestResult(result);
			}
			catch (...) {
				throw hresult_invalid_argument(SDKMessageLoginFailed);
			}
			SetEvent(event);
		};

		CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(
			CoreDispatcherPriority::Normal,
			std::bind(function, this, asyncEvent, permissions));

		co_await resume_on_signal(asyncEvent);

		co_return this->_asyncResult;
	}

	winsdkfb::FBResult FBSession::ExtractAccessTokenDataFromResponseData(
		IVectorView<WebTokenResponse> responseData
	) {
		winsdkfb::FBResult result;

		if (responseData.Size() > 0)
		{
			// Calculate a time 90 minutes from now.  This is the *earliest* time
			// at which our token will expire, so to be conservative we'll assume
			// that's when it expires.  The token broker doesn't expose the
			// actual expiration time, so this is the best we can do.
			//
			auto response = responseData.GetAt(0);

			Calendar cal;
			DateTime now = cal.GetDateTime();
			long long minimumExpiryInTicks = now.time_since_epoch().count() + _90_MINUTES_IN_TICKS;
			DateTime expiration = winrt::clock::from_time_t(minimumExpiryInTicks);
			FBAccessTokenData token(response.Token().data(), expiration, expiration);
			result = FBResult(token);

#ifdef _DEBUG
			hstring msg(L"Token is: " + response.Token() + L"\n");
			OutputDebugString(msg.c_str());

			for (auto const& item : response.Properties()) {
				msg = L"Found pair (" + item.Key() + L", " + item.Value() + L")\n";
				OutputDebugString(msg.c_str());
			}

			msg = L"User ID: " + response.WebAccount().Id() + L"\n";
			OutputDebugString(msg.c_str());

			msg = L"User Name: " + response.WebAccount().UserName() + L"\n";
			OutputDebugString(msg.c_str());

			for (auto const& item : response.WebAccount().Properties()) {
				msg = L"Found pair (" + item.Key() + L", " + item.Value() + L")\n";
				OutputDebugString(msg.c_str());
			}
#endif // _DEBUG
		}

		return result;
	}

	winsdkfb::FBResult FBSession::FBResultFromTokenRequestResult(
		WebTokenRequestResult requestResult
	) const {
		winsdkfb::FBResult result;

		if (requestResult != nullptr)
		{
			WebTokenRequestStatus status = requestResult.ResponseStatus();
			bool expectedError = false;

			// TODO: Do we need to handle any of the other status codes here in any
			// way besides just returning null?
			switch (status)
			{
			case WebTokenRequestStatus::Success:
				result = ExtractAccessTokenDataFromResponseData(requestResult.ResponseData());
				break;

			case WebTokenRequestStatus::UserCancel:
			case WebTokenRequestStatus::UserInteractionRequired:
			case WebTokenRequestStatus::AccountProviderNotAvailable:
			case WebTokenRequestStatus::AccountSwitch:
			case WebTokenRequestStatus::ProviderError:
				expectedError = true;
			default:
#ifdef _DEBUG
				if (!expectedError)
				{
					wchar_t buf[100];
					_itow_s(static_cast<int>(status), buf, 100, 10);
					wstring msg = L"Unexpected status result: " + wstring(buf) + L"\n";
					OutputDebugString(msg.c_str());
				}
#endif
				result = FBResult(FBError(static_cast<int>(ErrorCode::ErrorCodeWebTokenRequestStatus), std::wstring(L"WebTokenRequestStatus Error"), std::wstring(WebTokenRequestStatusToString(status)).c_str()));
				break;
			}
		}
		else
		{
			// We don't have a provider
			result = FBResult(FBError(static_cast<int>(ErrorCode::ErrorCodeWebAccountProviderNotFound), L"WebAccountProvider Error", L"No appropriate WebAccountProvider was found"));
		}

		return result;
	}
#endif

	auto FBSession::IsRerequest(unordered_map<wstring, wstring> parameters) -> bool {
		auto isRerequest = false;
		if (parameters.find(AuthTypeKey) != parameters.end()) {
			const auto parameter = parameters[AuthTypeKey];
			if (compare_ordinal(parameter.c_str(), Rerequest) == 0) {
				isRerequest = true;
			}
		}
		return isRerequest;
	}
}
