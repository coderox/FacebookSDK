﻿#include "pch.h"
#include <ppltasks.h>

#include "FacebookAccessTokenData.h"
#include "FacebookSession.h"
#include "FacebookAppRequest.h"
#include "FacebookDialog.h"
#include "FacebookError.h"
#include "FacebookFeedRequest.h"
#include "FacebookPaginatedArray.h"
#include "FacebookResult.h"
#include "Generated/Graph.FBPermission.h"
#include "FacebookSingleValue.h"
#include "Generated/Graph.FBUser.h"
#include "SDKMessage.h"
#include <regex>
#include "Utilities.h"
#include <sstream>
#include <string>

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
#define Reauthorize     L"reauthorize"
#define RedirectUriKey  L"redirect_uri"

#define SDK_APP_DATA_CONTAINER L"winsdkfb" // TODO: Should we move this?
#define GRANTED_PERMISSIONS_KEY L"granted_permissions"

namespace FacebookSDK
{
	HANDLE login_evt = NULL;
}

namespace winrt::FacebookSDK::implementation
{
	FacebookSession::FacebookSession()
		: _AccessTokenData(nullptr)
		, _loggedIn(false)
		, _user(nullptr)
		, _asyncResult(nullptr)
	{
		if (!::FacebookSDK::login_evt)
		{
			::FacebookSDK::login_evt = CreateEventEx(NULL, NULL, 0, DELETE | SYNCHRONIZE);
		}
		_APIMajorVersion = 2;
		_APIMinorVersion = 6;
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
		_webViewRedirectDomain = FACEBOOK_MOBILE_SERVER_NAME;
#else
		_webViewRedirectDomain = FACEBOOK_DESKTOP_SERVER_NAME;
#endif
		_webViewRedirectPath = FACEBOOK_LOGIN_SUCCESS_PATH;
	}

	FacebookSession::~FacebookSession()
	{
		if (::FacebookSDK::login_evt)
		{
			CloseHandle(::FacebookSDK::login_evt);
			::FacebookSDK::login_evt = NULL;
		}
	}


	hstring FacebookSession::FacebookAppId()
	{
		if (_FBAppId.empty())
		{
			throw hresult_invalid_argument(SDKMessageMissingAppID);
		}
		return _FBAppId.c_str();
	}

	void FacebookSession::FacebookAppId(hstring const& value)
	{
		_FBAppId = value.c_str();
	}

	hstring FacebookSession::WinAppId()
	{
		return _WinAppId.c_str();
	}

	void FacebookSession::WinAppId(hstring const& value)
	{
		_WinAppId = value.c_str();
	}

	hstring FacebookSession::AppResponse()
	{
		return _AppResponse.c_str();
	}

	bool FacebookSession::LoggedIn()
	{
		return _loggedIn;
	}

	FacebookSDK::FacebookAccessTokenData FacebookSession::AccessTokenData()
	{
		return _AccessTokenData;
	}

	void FacebookSession::AccessTokenData(FacebookSDK::FacebookAccessTokenData const& value)
	{
		_AccessTokenData = value;

		// If token have been updated, make sure to save updated token
		TrySaveTokenData();
	}

	int32_t FacebookSession::APIMajorVersion()
	{
		return _APIMajorVersion;
	}

	int32_t FacebookSession::APIMinorVersion()
	{
		return _APIMinorVersion;
	}

	FacebookSDK::Graph::FBUser FacebookSession::User()
	{
		return _user;
	}

	hstring FacebookSession::WebViewRedirectDomain()
	{
		return _webViewRedirectDomain.c_str();
	}

	hstring FacebookSession::WebViewRedirectPath()
	{
		return _webViewRedirectPath.c_str();
	}

	IAsyncAction FacebookSession::LogoutAsync()
	{
		_user = nullptr;
		_FBAppId.clear();
		_WinAppId.clear();
		_AccessTokenData = nullptr;
		_AppResponse.clear();
		_loggedIn = false;

		FacebookDialog::DeleteCookies();

		return TryDeleteTokenDataAsync();
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowFeedDialogAsync(PropertySet const Parameters)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		FacebookDialog dialog;
		try {
			result = co_await dialog.ShowFeedDialogAsync(Parameters);
		}
		catch (hresult_error e) {
			auto err = FacebookError::FromJson(hstring(ErrorObjectJson));
			result = make<FacebookResult>(err);
		}

		co_return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowRequestsDialogAsync(PropertySet const Parameters)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		FacebookDialog dialog;
		try {
			result = co_await dialog.ShowRequestsDialogAsync(Parameters);
		}
		catch (hresult_error e) {
			auto err = FacebookError::FromJson(hstring(ErrorObjectJson));
			result = make<FacebookResult>(err);
		}

		co_return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowSendDialogAsync(PropertySet const Parameters)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		FacebookDialog dialog;
		try {
			result = co_await dialog.ShowSendDialogAsync(Parameters);
		}
		catch (hresult_error e) {
			auto err = FacebookError::FromJson(hstring(ErrorObjectJson));
			result = make<FacebookResult>(err);
		}

		co_return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync()
	{
		return LoginAsync(nullptr, FacebookSDK::SessionLoginBehavior::DefaultOrdering);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync(FacebookSDK::FacebookPermissions permissions)
	{
		return LoginAsync(permissions, FacebookSDK::SessionLoginBehavior::DefaultOrdering);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync(FacebookSDK::FacebookPermissions permissions, FacebookSDK::SessionLoginBehavior behavior)
	{
		if (!permissions) {
			permissions = make<FacebookPermissions>();
		}

		PropertySet parameters;
		parameters.Insert(ScopeKey, box_value(permissions.ToString()));

		if (LoggedIn()) {
			parameters.Insert(AuthTypeKey, box_value(Rerequest));
		}
		FacebookSDK::FacebookResult result{ nullptr };
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

			if (_asyncResult == nullptr || (_asyncResult.ErrorInfo() != nullptr && _asyncResult.ErrorInfo().Code() == (int)ErrorCode::ErrorCodeWebAccountProviderNotFound)) {
				_asyncResult = co_await TryLoginViaWebViewAsync(parameters);
				if (_asyncResult == nullptr || (_asyncResult.ErrorInfo() != nullptr)) {
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
			_asyncResult = make<FacebookResult>(make<FacebookError>(0, L"Login Error", L"Invalid SessionLoginBehavior member"));
			break;
		}

		auto userInfoResult = co_await TryGetUserInfoAfterLoginAsync(_asyncResult);
		auto finalResult = co_await TryGetAppPermissionsAfterLoginAsync(userInfoResult);

		if (finalResult == nullptr || !finalResult.Succeeded()) {
			_loggedIn = false;
			AccessTokenData(nullptr);
			co_await TryDeleteTokenDataAsync();

			if (finalResult == nullptr) {
				finalResult = make<FacebookResult>(make<FacebookError>(0, L"Unexpected error", L"Log in attempt failed"));
				OutputDebugString(L"LoginAsync was about to return nullptr, created FacebookResult object to return instead");
			}
		}
		else {
			SaveGrantedPermissions();
		}
		co_return finalResult;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ReauthorizeAsync(FacebookSDK::FacebookPermissions permissions)
	{
		if (!LoggedIn()) {
			return LoginAsync(permissions, FacebookSDK::SessionLoginBehavior::DefaultOrdering);
		}
		else {
			if (!permissions) {
				permissions = make<FacebookPermissions>();
			}

			PropertySet parameters;
			parameters.Insert(ScopeKey, box_value(permissions.ToString()));

			parameters.Insert(AuthTypeKey, box_value(Reauthorize));

			FacebookSDK::FacebookResult result{ nullptr };

			_asyncResult = co_await TryLoginViaWebViewAsync(parameters);

			auto userInfoResult = co_await TryGetUserInfoAfterLoginAsync(_asyncResult);
			auto finalResult = co_await TryGetAppPermissionsAfterLoginAsync(userInfoResult);

			if (finalResult == nullptr || !finalResult.Succeeded()) {
				_loggedIn = false;
				AccessTokenData(nullptr);

				if (finalResult == nullptr) {
					finalResult = make<FacebookResult>(make<FacebookError>(0, L"Unexpected error", L"Reauthorize attempt failed"));
					OutputDebugString(L"ReauthorizeAsync was about to return nullptr, created FacebookResult object to return instead");
				}
			}
			else {
				SaveGrantedPermissions();
			}
			co_return finalResult;
		}
	}

	void FacebookSession::SetApiVersion(int32_t major, int32_t minor)
	{
		_APIMajorVersion = major;
		_APIMinorVersion = minor;
	}

	void FacebookSession::SetWebViewRedirectUrl(hstring const& domain, hstring const& path)
	{
		if (!domain.empty()) {
			_webViewRedirectDomain = domain.c_str();
		}
		if (!path.empty()) {
			_webViewRedirectPath = path.c_str();
		}
	}

	FacebookSDK::FacebookSession FacebookSession::ActiveSession()
	{
		static FacebookSDK::FacebookSession activeFBSession = make<FacebookSession>();
		return activeFBSession;
	}

	ApplicationDataContainer FacebookSession::DataContainer()
	{
		ApplicationDataContainer localSettings = ApplicationData::Current().LocalSettings();
		if (!localSettings.Containers().HasKey(SDK_APP_DATA_CONTAINER))
		{
			localSettings.CreateContainer(SDK_APP_DATA_CONTAINER, ApplicationDataCreateDisposition::Always);
		}
		return localSettings.Containers().Lookup(SDK_APP_DATA_CONTAINER);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryRefreshAccessTokenAsync()
	{
		WebTokenRequestResult result{ nullptr };

		auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
		if (provider) {
			auto perms = GetGrantedPermissions();
			WebTokenRequest request(provider, perms, FacebookAppId());
			request.Properties().Insert(RedirectUriKey, GetWebAccountProviderRedirectUriString());
			result = co_await WebAuthenticationCoreManager::GetTokenSilentlyAsync(request);
			if (result) {
				co_await result.InvalidateCacheAsync();

			}
		}

		co_return FacebookResultFromTokenRequestResult(result);
	}

	Uri FacebookSession::BuildLoginUri(PropertySet parameters)
	{
		auto session = FacebookSession::ActiveSession();
		hstring apiVersion(L"");
		if (APIMajorVersion())
		{
			wstringstream apiStream;
			apiStream << L"v" + APIMajorVersion() << L"." << APIMinorVersion() << L"/";
			apiVersion = apiStream.str().c_str();
		}
		wstringstream uriStream;
		uriStream << L"https://www.facebook.com/" << apiVersion.c_str() << L"dialog/oauth?client_id=" << session.FacebookAppId().c_str();
		//hstring uriString(uriStream.str().c_str());

		// Use some reasonable default login parameters
		hstring scope(DefaultScope);
		hstring displayType(DefaultDisplay);
		hstring responseType(DefaultResponse);

		uriStream << L"&" << RedirectUriKey << L"=" << GetWebAuthRedirectUriString().c_str();

		for (auto const& parameter : parameters) {
			hstring key = parameter.Key();
			auto value = unbox_value<hstring>(parameter.Value());
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

	hstring FacebookSession::GetWebAuthRedirectUriString() {
		Uri endURI(WebAuthenticationBroker::GetCurrentApplicationCallbackUri());
		return endURI.DisplayUri();
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::GetUserInfoAsync(
		FacebookSDK::FacebookAccessTokenData const& tokenData
	) {
		PropertySet parameters;
		parameters.Insert(L"fields", box_value(L"gender,link,first_name,last_name,locale,timezone,email,updated_time,verified,name,id,picture"));
		auto objectFactory = JsonClassFactory([](hstring jsonText) -> IInspectable
			{
				return Graph::FBUser::FromJson(jsonText);
			});

		FacebookSDK::FacebookSingleValue value = make<FacebookSingleValue>(
			L"/me",
			parameters,
			objectFactory);

		auto result = co_await value.GetAsync();
		co_return result;
	}

	void FacebookSession::ParseOAuthResponse(Uri ResponseUri) {
		throw hresult_not_implemented();
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::CheckForExistingTokenAsync()
	{
		FacebookSDK::FacebookResult result{ nullptr };
		if (LoggedIn())
		{
			result = make<FacebookResult>(this->AccessTokenData());
		}
		else
		{
			try {
				auto folder = ApplicationData::Current().LocalFolder();
				IStorageItem item = co_await folder.TryGetItemAsync(L"FBSDKData");
				if (item != nullptr) {
					auto file = item.as<StorageFile>();
					auto protectedBuffer = co_await FileIO::ReadBufferAsync(file);
					DataProtectionProvider provider;
					auto clearBuffer = co_await provider.UnprotectAsync(protectedBuffer);
					auto clearText = CryptographicBuffer::ConvertBinaryToString(BinaryStringEncoding::Utf16LE, clearBuffer);

					wstring vals(clearText.c_str());
					size_t pos = vals.find(L",");

					if (pos != wstring::npos)
					{
						DateTime expirationTime;
						DateTime dataAccessExpirationTime;
						hstring accessToken(vals.substr(0, pos).c_str());
						hstring msg(L"Access Token: " + accessToken + L"\n");
						OutputDebugString(msg.c_str());

						hstring expirationString(vals.substr(pos + 1, wstring::npos).c_str());
						expirationTime = winrt::clock::from_time_t(_wtoi64(expirationString.c_str()));

						FacebookSDK::FacebookAccessTokenData cachedData{ nullptr };

						pos = vals.find(L",", pos);
						if (pos != wstring::npos)
						{
							hstring dataExpirationString(vals.substr(pos + 1, wstring::npos).c_str());
							dataAccessExpirationTime = winrt::clock::from_time_t(_wtoi64(dataExpirationString.c_str()));
							cachedData = make<FacebookAccessTokenData>(accessToken, expirationTime, dataAccessExpirationTime);
						}
						else {
#ifdef _DEBUG
							OutputDebugString(L"Read token without data access expiration time!\n");
#endif
							cachedData = make<FacebookAccessTokenData>(accessToken, expirationTime);
						}
						result = make<FacebookResult>(cachedData);
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

	fire_and_forget FacebookSession::TrySaveTokenData() {
		if (LoggedIn())
		{
			wchar_t buffer[INT64_STRING_BUFSIZE];
			DataProtectionProvider provider(L"LOCAL=user");
			_i64tow_s(
				WindowsTickToUnixSeconds(AccessTokenData().ExpirationDate().time_since_epoch().count()),
				buffer, INT64_STRING_BUFSIZE, 10);
			wstringstream tokenStream;
			tokenStream << AccessTokenData().AccessToken().c_str() << L"," << hstring(buffer).c_str();
			if (AccessTokenData().HasDataAccessExpired()) {
				_i64tow_s(
					WindowsTickToUnixSeconds(AccessTokenData().DataAccessExpirationDate().time_since_epoch().count()),
					buffer, INT64_STRING_BUFSIZE, 10);
				tokenStream << L"," << hstring(buffer).c_str();
			}
			hstring tokenData(tokenStream.str().c_str());
			IBuffer dataBuff = CryptographicBuffer::ConvertStringToBinary(tokenData, BinaryStringEncoding::Utf16LE);

			auto protectedData = co_await provider.ProtectAsync(dataBuff);
			StorageFolder folder = ApplicationData::Current().LocalFolder();
			auto file = co_await folder.CreateFileAsync(L"FBSDKData", CreationCollisionOption::OpenIfExists);
			co_await FileIO::WriteBufferAsync(file, protectedData);
		}
	}

	IAsyncAction FacebookSession::TryDeleteTokenDataAsync() {
		StorageFolder folder = ApplicationData::Current().LocalFolder();
#ifdef _DEBUG
		wstringstream msgStream;
		msgStream << L"Deleting cached token from " << folder.Path().c_str() << L"\n";
		hstring msg(msgStream.str().c_str());
		OutputDebugString(msg.c_str());
#endif
		try {
			auto item = co_await folder.TryGetItemAsync(L"FBSDKData");
			if (item) {
				item.DeleteAsync();
			}
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

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::GetAppPermissionsAsync()
	{
		wstringstream permStream;
		permStream << L"/" << _user.Id().c_str() << L"/permissions";
		FacebookPaginatedArray permArr(
			permStream.str().c_str(),
			nullptr,
			JsonClassFactory([](hstring const& JsonText) -> IInspectable
				{
					return Graph::FBPermission::FromJson(JsonText);
				}));

		auto result = co_await permArr.FirstAsync();
		if (result.Succeeded()) {
			auto perms = result.Object().as<IVectorView<IInspectable>>();
			_AccessTokenData.SetPermissions(perms);
		}
		co_return make<FacebookResult>(_user);
	}

	FacebookSDK::FacebookResult FacebookSession::ProcessAuthResult(WebAuthenticationResult authResult)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		hstring uriString;
		FacebookSDK::FacebookAccessTokenData tokenData{ nullptr };
		Uri uri{ nullptr };

		switch (authResult.ResponseStatus())
		{
		case WebAuthenticationStatus::ErrorHttp:
			//TODO: need a real error code
			result = make<FacebookResult>(make<FacebookError>(0,
				L"Communication error",
				L"An Http error occurred"));
			break;
		case WebAuthenticationStatus::Success:
			//TODO: need a real error code
			uriString = authResult.ResponseData();
			uri = Uri(uriString);
			tokenData = FacebookAccessTokenData::FromUri(uri);
			if (!tokenData)
			{
				result = make<FacebookResult>(FacebookError::FromUri(uri));
			}
			else
			{
				result = make<FacebookResult>(tokenData);
			}
			break;
		case WebAuthenticationStatus::UserCancel:
			result = make<FacebookResult>(make<FacebookError>(0,
				L"User canceled",
				L"The login operation was canceled"));
			break;
		default:
			break;
		}
		return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryGetUserInfoAfterLoginAsync(FacebookSDK::FacebookResult loginResult)
	{
		FacebookSDK::FacebookResult result{ nullptr };

		if (loginResult != nullptr && loginResult.Succeeded())
		{
			_AccessTokenData = loginResult.Object().as<FacebookSDK::FacebookAccessTokenData>();
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

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryGetAppPermissionsAfterLoginAsync(FacebookSDK::FacebookResult loginResult)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		if (loginResult != nullptr && loginResult.Succeeded())
		{
			_user = loginResult.Object().as<Graph::FBUser>();
			result = co_await GetAppPermissionsAsync();
		}
		else
		{
			result = loginResult;
		}

		co_return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::RunOAuthOnUiThreadAsync(PropertySet Parameters)
	{
		try
		{
			_asyncResult = nullptr;
			auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
			auto function = [](FacebookSession *self, HANDLE event, PropertySet parameters) -> IAsyncAction {
				auto authResult = co_await WebAuthenticationBroker::AuthenticateAsync(
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

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::RunWebViewLoginOnUIThreadAsync(PropertySet Parameters)
	{
		try
		{
			_asyncResult = nullptr;
			auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
			auto function = [](FacebookSession *self, HANDLE event, PropertySet parameters) -> IAsyncAction {
				auto result = co_await self->ShowLoginDialogAsync(parameters);
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

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowLoginDialogAsync(PropertySet const& Parameters)
	{
		FacebookSDK::FacebookResult result{ nullptr };
		FacebookDialog dialog;
		try {
			result = co_await dialog.ShowLoginDialogAsync(Parameters);
		}
		catch (hresult_error e) {
			auto err = FacebookError::FromJson(hstring(ErrorObjectJson));
			result = make<FacebookResult>(err);
		}

		if (result.Succeeded())
		{
			AccessTokenData(result.Object().as<FacebookSDK::FacebookAccessTokenData>());
		}

		co_return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebViewAsync(
		PropertySet parameters
	) {
		FacebookSDK::FacebookResult loginResult{ nullptr };
		auto session = FacebookSession::ActiveSession();

		if (!IsRerequest(parameters)) {
			auto oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult != nullptr && oauthResult.Succeeded()) {
				if (FacebookSDK::FacebookAccessTokenData tokenData{ oauthResult.Object().try_as<FacebookSDK::FacebookAccessTokenData>() }) {
					if (!tokenData.IsExpired()) {
						loginResult = make<FacebookResult>(tokenData);
					}
				}
			}
			else {
				loginResult = co_await RunWebViewLoginOnUIThreadAsync(parameters);
			}
		}
		co_return loginResult;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebAuthBrokerAsync(
		PropertySet parameters
	) {
		FacebookSDK::FacebookResult loginResult{ nullptr };
		auto session = FacebookSession::ActiveSession();

		if (!IsRerequest(parameters)) {
			auto oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult != nullptr && oauthResult.Succeeded()) {
				auto tokenData = oauthResult.Object().as<FacebookSDK::FacebookAccessTokenData>();
				if (tokenData != nullptr && !tokenData.IsExpired()) {
					loginResult = make<FacebookResult>(tokenData);
				}
			}
			else {
				loginResult = co_await RunOAuthOnUiThreadAsync(parameters);
			}
		}
		co_return loginResult;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryLoginSilentlyAsync(
		PropertySet parameters
	) {
		FacebookSDK::FacebookResult loginResult{ nullptr };
		auto session = FacebookSession::ActiveSession();

		auto grantedPermissions = FacebookPermissions::FromString(GetGrantedPermissions());
		auto requestingPermissions = FacebookPermissions::FromString(unbox_value<hstring>(parameters.Lookup(L"scope")));

		OutputDebugString(hstring(L"Granted permissions: " + grantedPermissions.ToString() + L"\n").c_str());
		OutputDebugString(hstring(L"Requested permissions: " + requestingPermissions.ToString() + L"\n").c_str());

		auto diffPermissions = FacebookPermissions::Difference(requestingPermissions, grantedPermissions);

		FacebookSDK::FacebookResult oauthResult{ nullptr };
		if (diffPermissions.Values().Size() == 0) {
			oauthResult = co_await CheckForExistingTokenAsync();
			if (oauthResult != nullptr && oauthResult.Succeeded()) {
				auto tokenData = oauthResult.Object().as<FacebookSDK::FacebookAccessTokenData>();
				if (tokenData != nullptr && !tokenData.IsExpired()) {
					loginResult = make<FacebookResult>(tokenData);
				}
				else {
					loginResult = make<FacebookResult>(make<FacebookError>(0, L"Restore Session Error", L"Could not find a valid access token or token has expired"));
				}
			}
			else {
				loginResult = make<FacebookResult>(make<FacebookError>(0, L"Restore Session Error", L"Could not find a valid access token"));
			}
		}
		else {
			loginResult = make<FacebookResult>(make<FacebookError>(0, L"Restore Session Error", L"Requested permissions are different from granted"));
		}
		co_return loginResult;
	}

	void FacebookSession::SaveGrantedPermissions() {
		auto values = FacebookSession::DataContainer().Values();
		auto grantedPermissions = AccessTokenData().GrantedPermissions().ToString();
		OutputDebugString(hstring(L"Granted permissions: " + grantedPermissions + L"\n").c_str());
		values.Insert(GRANTED_PERMISSIONS_KEY, box_value(grantedPermissions));
	}

	hstring FacebookSession::GetGrantedPermissions() {
		auto values = FacebookSession::DataContainer().Values();
		if (!values.HasKey(GRANTED_PERMISSIONS_KEY)) {
			return L"";
		}
		return unbox_value<hstring>(values.Lookup(GRANTED_PERMISSIONS_KEY));
	}

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	hstring FacebookSession::GetWebAccountProviderRedirectUriString() {
		Package package = Package::Current();
		PackageId packageId = package.Id();
		//hstring phoneAppId = packageId.ProductId();
		hstring phoneAppId = L"s-1-15-2-993894519-2096754445-998123955-3150536948-1224592709-2849448323-2973583169";
		return L"msft-" + phoneAppId + L"://login_success";
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::CheckWebAccountProviderForExistingTokenAsync(
		FacebookSDK::FacebookPermissions permissions
	) {
		FacebookSDK::FacebookResult result{ nullptr };
		if (LoggedIn())
		{
			co_return make<FacebookResult>(AccessTokenData());
		}
		else
		{
			auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
			if (provider != nullptr) {
				WebTokenRequest request(provider, permissions.ToString(), FacebookAppId());
				request.Properties().Insert(RedirectUriKey, GetWebAccountProviderRedirectUriString());
				auto requestResult = co_await WebAuthenticationCoreManager::GetTokenSilentlyAsync(request);

				if (requestResult && requestResult.ResponseStatus() == WebTokenRequestStatus::UserInteractionRequired)
				{
					result = co_await CallWebAccountProviderOnUiThreadAsync(permissions);
				}
				else {
					result = FacebookResultFromTokenRequestResult(requestResult);
				}
			}
		}

		return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebAccountProviderAsync(
		FacebookSDK::FacebookPermissions permissions
	) {
		return CheckWebAccountProviderForExistingTokenAsync(permissions);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::CallWebAccountProviderOnUiThreadAsync(
		FacebookSDK::FacebookPermissions permissions
	) {
		_asyncResult = nullptr;
		auto asyncEvent = CreateEvent(nullptr, true, false, nullptr);
		auto function = [](FacebookSession *self, HANDLE event, FacebookSDK::FacebookPermissions const& permissions) -> IAsyncAction {
			try {
				auto provider = co_await WebAuthenticationCoreManager::FindAccountProviderAsync(FBAccountProvider);
				WebTokenRequestResult result = nullptr;
				if (provider) {
					WebTokenRequest request(provider, permissions.ToString(), self->FacebookAppId());
					request.Properties().Insert(RedirectUriKey, self->GetWebAccountProviderRedirectUriString());
					result = co_await WebAuthenticationCoreManager::RequestTokenAsync(request);
				}
				self->_asyncResult = self->FacebookResultFromTokenRequestResult(result);
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

	FacebookSDK::FacebookResult FacebookSession::ExtractAccessTokenDataFromResponseData(
		IVectorView<WebTokenResponse> responseData
	) {
		FacebookSDK::FacebookResult result{ nullptr };

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
			FacebookAccessTokenData token(response.Token(), expiration);
			result = make<FacebookResult>(token);

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

	FacebookSDK::FacebookResult FacebookSession::FacebookResultFromTokenRequestResult(
		WebTokenRequestResult requestResult
	) {
		FacebookSDK::FacebookResult result{ nullptr };

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
					_itow_s((int)status, buf, 100, 10);
					hstring msg = L"Unexpected status result: " + hstring(buf) + L"\n";
					OutputDebugString(msg.c_str());
				}
#endif
				result = make<FacebookResult>(make<FacebookError>((int)ErrorCode::ErrorCodeWebTokenRequestStatus, L"WebTokenRequestStatus Error", WebTokenRequestStatusToString(status)));
				break;
			}
		}
		else
		{
			// We don't have a provider
			result = make<FacebookResult>(make<FacebookError>((int)ErrorCode::ErrorCodeWebAccountProviderNotFound, L"WebAccountProvider Error", L"No appropriate WebAccountProvider was found"));
		}

		return result;
	}
#endif

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	Windows::Foundation::Ur FacebookSession::RemoveJSONFromBrowserResponseUri(
		Windows::Foundation::Uri responseUri
	) {
		throw hresult_not_implemented();
	}
#endif

	BOOL FacebookSession::IsRerequest(PropertySet parameters) {
		bool isRerequest = false;
		if (parameters != nullptr && parameters.HasKey(AuthTypeKey)) {
			hstring value = unbox_value<hstring>(parameters.Lookup(AuthTypeKey));
			if (compare_ordinal(value.c_str(), Rerequest) == 0) {
				isRerequest = true;
			}
		}
		return isRerequest;
	}
}
