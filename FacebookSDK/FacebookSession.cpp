#include "pch.h"
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

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
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
		, _AppResponse(nullptr) 
		, _loggedIn(false)
		, _FBAppId(nullptr)
		, _WinAppId(nullptr)
		, _user(nullptr)
		, _webViewRedirectDomain(nullptr)
		, _webViewRedirectPath(nullptr)
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
		throw hresult_not_implemented();
	}

	int32_t FacebookSession::APIMinorVersion()
	{
		throw hresult_not_implemented();
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

	Windows::Foundation::IAsyncAction FacebookSession::LogoutAsync()
	{
		_user = nullptr;
		_FBAppId.clear();
		_WinAppId.clear();
		_AccessTokenData = nullptr;
		_AppResponse.clear();
		_loggedIn = false;

		FacebookDialog::DeleteCookies();

		return TryDeleteTokenData();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync(FacebookSDK::FacebookPermissions const permissions)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::LoginAsync(FacebookSDK::FacebookPermissions const permissions, FacebookSDK::SessionLoginBehavior const behavior)
	{
		throw hresult_not_implemented();
	}

	void FacebookSession::SetApiVersion(int32_t major, int32_t minor)
	{
		throw hresult_not_implemented();
	}

	void FacebookSession::SetWebViewRedirectUrl(hstring const& domain, hstring const& Path)
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookSession FacebookSession::ActiveSession()
	{
		throw hresult_not_implemented();
	}

	Windows::Storage::ApplicationDataContainer FacebookSession::DataContainer()
	{
		ApplicationDataContainer localSettings = ApplicationData::Current().LocalSettings();
		if (!localSettings.Containers().HasKey(SDK_APP_DATA_CONTAINER))
		{
			localSettings.CreateContainer(SDK_APP_DATA_CONTAINER, ApplicationDataCreateDisposition::Always);
		}
		return localSettings.Containers().Lookup(SDK_APP_DATA_CONTAINER);
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryRefreshAccessTokenAsync()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::Uri FacebookSession::BuildLoginUri(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	hstring FacebookSession::GetWebAuthRedirectUriString() {
		throw hresult_not_implemented();
	}
	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::GetUserInfo(
		FacebookSDK::FacebookAccessTokenData const& tokenData
	) {
		throw hresult_not_implemented();
	}

	void FacebookSession::ParseOAuthResponse(
		Windows::Foundation::Uri ResponseUri
	) {
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<Windows::Storage::IStorageItem>
		FacebookSession::MyTryGetItemAsync(
			Windows::Storage::StorageFolder folder,
			hstring itemName
		) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::CheckForExistingToken() {
		throw hresult_not_implemented();
	}

	void FacebookSession::TrySaveTokenData() {
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncAction FacebookSession::TryDeleteTokenData() {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::GetAppPermissions(
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult>
		FacebookSession::ProcessAuthResult(
			Windows::Security::Authentication::Web::WebAuthenticationResult authResult
		) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryGetUserInfoAfterLogin(
		FacebookSDK::FacebookResult loginResult
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryGetAppPermissionsAfterLogin(
		FacebookSDK::FacebookResult loginResult
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::RunOAuthOnUiThread(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::RunWebViewLoginOnUIThread(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::ShowLoginDialog(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebView(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebAuthBroker(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryLoginSilently(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}

	void FacebookSession::SaveGrantedPermissions() {
		throw hresult_not_implemented();
	}

	hstring FacebookSession::GetGrantedPermissions() {
		throw hresult_not_implemented();
	}

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	hstring FacebookSession::GetWebAccountProviderRedirectUriString() {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::CheckWebAccountProviderForExistingToken(
		FacebookSDK::FacebookPermissions Permissions
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::TryLoginViaWebAccountProvider(
		FacebookSDK::FacebookPermissions Permissions
	) {
		throw hresult_not_implemented();
	}

	concurrency::task<FacebookSDK::FacebookResult> FacebookSession::CallWebAccountProviderOnUiThread(
		FacebookSDK::FacebookPermissions Permissions
	) {
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookResult FacebookSession::ExtractAccessTokenDataFromResponseData(
		Windows::Foundation::Collections::IVectorView
		<Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData
	) {
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookResult FacebookSession::FBResultFromTokenRequestResult(
		Windows::Security::Authentication::Web::Core::WebTokenRequestResult RequestResult
	) {
		throw hresult_not_implemented();
	}
#endif

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	Windows::Foundation::Ur FacebookSession::RemoveJSONFromBrowserResponseUri(
		Windows::Foundation::Uri responseUri
	) {
		throw hresult_not_implemented();
	}
#endif

	BOOL FacebookSession::IsRerequest(
		Windows::Foundation::Collections::PropertySet Parameters
	) {
		throw hresult_not_implemented();
	}
}
