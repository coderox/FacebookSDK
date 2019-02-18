#pragma once

#include "FacebookSession.g.h"
#include <ppltasks.h>

namespace winrt::winsdkfb::implementation
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

		winsdkfb::FacebookAccessTokenData AccessTokenData();
		void AccessTokenData(winsdkfb::FacebookAccessTokenData const& value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		winsdkfb::Graph::FBUser User();

		hstring WebViewRedirectDomain();
		hstring WebViewRedirectPath();

		Windows::Foundation::IAsyncAction LogoutAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> LoginAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> LoginAsync(winsdkfb::FacebookPermissions permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> LoginAsync(winsdkfb::FacebookPermissions permissions, winsdkfb::SessionLoginBehavior behavior);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryRefreshAccessTokenAsync();

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(hstring const& domain, hstring const& Path);

		static winsdkfb::FacebookSession ActiveSession();
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
		winsdkfb::FacebookResult ProcessAuthResult(Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> GetUserInfoAsync(winsdkfb::FacebookAccessTokenData const& tokenData);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> CheckForExistingTokenAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> GetAppPermissionsAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const& Parameters);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryGetUserInfoAfterLoginAsync(winsdkfb::FacebookResult loginResult);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryGetAppPermissionsAfterLoginAsync(winsdkfb::FacebookResult loginResult);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> RunOAuthOnUiThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> RunWebViewLoginOnUIThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryLoginViaWebViewAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryLoginViaWebAuthBrokerAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryLoginSilentlyAsync(Windows::Foundation::Collections::PropertySet Parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		hstring GetWebAccountProviderRedirectUriString();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FacebookPermissions Permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> TryLoginViaWebAccountProviderAsync(winsdkfb::FacebookPermissions Permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FacebookPermissions Permissions);
		winsdkfb::FacebookResult ExtractAccessTokenDataFromResponseData(Windows::Foundation::Collections::IVectorView<Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData);
		winsdkfb::FacebookResult FacebookResultFromTokenRequestResult(Windows::Security::Authentication::Web::Core::WebTokenRequestResult RequestResult);
#endif

		bool _loggedIn;
		int _APIMajorVersion;
		int _APIMinorVersion;
		std::wstring _FBAppId;
		std::wstring _WinAppId;
		std::wstring _AppResponse;
		std::wstring _webViewRedirectDomain;
		std::wstring _webViewRedirectPath;
		winsdkfb::FacebookAccessTokenData _AccessTokenData{ nullptr };
		winsdkfb::Graph::FBUser _user{ nullptr };
		concurrency::task<winsdkfb::FacebookResult> _loginTask;
		winsdkfb::FacebookDialog _dialog{ nullptr };
		winsdkfb::FacebookResult _asyncResult{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookSession : FacebookSessionT<FacebookSession, implementation::FacebookSession>
	{
	};
}
