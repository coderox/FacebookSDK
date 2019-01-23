#pragma once

#include "FacebookSession.g.h"
#include <ppltasks.h>

namespace winrt::FacebookSDK::implementation
{
	struct FacebookSession : FacebookSessionT<FacebookSession>
	{
		FacebookSession();

		hstring FacebookAppId();
		void FacebookAppId(hstring const& value);

		hstring WinAppId();
		void WinAppId(hstring const& value);

		hstring AppResponse();

		bool LoggedIn();

		FacebookSDK::FacebookAccessTokenData AccessTokenData();
		void AccessTokenData(FacebookSDK::FacebookAccessTokenData const& value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		FacebookSDK::Graph::FBUser User();

		hstring WebViewRedirectDomain();
		hstring WebViewRedirectPath();

		Windows::Foundation::IAsyncAction LogoutAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> LoginAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> LoginAsync(FacebookSDK::FacebookPermissions const permissions);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> LoginAsync(FacebookSDK::FacebookPermissions const permissions, FacebookSDK::SessionLoginBehavior const behavior);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryRefreshAccessTokenAsync();

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(hstring const& domain, hstring const& Path);

		static FacebookSDK::FacebookSession ActiveSession();
		static Windows::Storage::ApplicationDataContainer DataContainer();

	private:

		~FacebookSession();

		void ParseOAuthResponse(Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		Windows::Foundation::Uri BuildLoginUri(Windows::Foundation::Collections::PropertySet parameters);

		BOOL IsRerequest(Windows::Foundation::Collections::PropertySet Parameters);

		hstring GetGrantedPermissions();
		hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		FacebookSDK::FacebookResult ProcessAuthResult(Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		Windows::Foundation::IAsyncOperation<Windows::Storage::IStorageItem> MyTryGetItemAsync(Windows::Storage::StorageFolder folder, hstring itemName);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> GetUserInfoAsync(FacebookSDK::FacebookAccessTokenData const& tokenData);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> CheckForExistingTokenAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> GetAppPermissionsAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const& Parameters);

		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryGetUserInfoAfterLoginAsync(FacebookSDK::FacebookResult loginResult);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryGetAppPermissionsAfterLoginAsync(FacebookSDK::FacebookResult loginResult);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> RunOAuthOnUiThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> RunWebViewLoginOnUIThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryLoginViaWebViewAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryLoginViaWebAuthBrokerAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryLoginSilentlyAsync(Windows::Foundation::Collections::PropertySet Parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		hstring GetWebAccountProviderRedirectUriString();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> CheckWebAccountProviderForExistingTokenAsync(FacebookSDK::FacebookPermissions Permissions);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryLoginViaWebAccountProviderAsync(FacebookSDK::FacebookPermissions Permissions);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> CallWebAccountProviderOnUiThreadAsync(FacebookSDK::FacebookPermissions Permissions);
		FacebookSDK::FacebookResult ExtractAccessTokenDataFromResponseData(Windows::Foundation::Collections::IVectorView<Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData);
		FacebookSDK::FacebookResult FBResultFromTokenRequestResult(Windows::Security::Authentication::Web::Core::WebTokenRequestResult RequestResult);
#endif

		bool _loggedIn;
		int _APIMajorVersion;
		int _APIMinorVersion;
		std::wstring _FBAppId;
		std::wstring _WinAppId;
		std::wstring _AppResponse;
		std::wstring _webViewRedirectDomain;
		std::wstring _webViewRedirectPath;
		FacebookSDK::FacebookAccessTokenData _AccessTokenData{ nullptr };
		FacebookSDK::Graph::FBUser _user{ nullptr };
		concurrency::task<FacebookSDK::FacebookResult> _loginTask;
		FacebookSDK::FacebookDialog _dialog{ nullptr };
		FacebookSDK::FacebookResult _asyncResult{ nullptr };
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookSession : FacebookSessionT<FacebookSession, implementation::FacebookSession>
	{
	};
}
