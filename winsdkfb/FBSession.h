#pragma once

#include "FBSession.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBSession : FBSessionT<FBSession>
	{
		FBSession();
		~FBSession();

		hstring FBAppId();
		void FBAppId(hstring const& value);

		hstring WinAppId();
		void WinAppId(hstring const& value);

		hstring AppResponse();

		bool LoggedIn();

		winsdkfb::FBAccessTokenData AccessTokenData();
		void AccessTokenData(winsdkfb::FBAccessTokenData const& value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		winsdkfb::Graph::FBUser User();

		hstring WebViewRedirectDomain();
		hstring WebViewRedirectPath();

		winrt::Windows::Foundation::IAsyncAction LogoutAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowFeedDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowRequestsDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowSendDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions, winsdkfb::SessionLoginBehavior behavior);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryRefreshAccessTokenAsync();

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(hstring const& domain, hstring const& Path);

		static winsdkfb::FBSession ActiveSession();
		static Windows::Storage::ApplicationDataContainer DataContainer();

	private:
		void ParseOAuthResponse(Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		Windows::Foundation::Uri BuildLoginUri(Windows::Foundation::Collections::PropertySet parameters);

		BOOL IsRerequest(winrt::Windows::Foundation::Collections::PropertySet Parameters);

		hstring GetGrantedPermissions();
		hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		winrt::Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		winsdkfb::FBResult ProcessAuthResult(Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetUserInfoAsync(winsdkfb::FBAccessTokenData const& tokenData);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CheckForExistingTokenAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetAppPermissionsAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowLoginDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const& Parameters);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryGetUserInfoAfterLoginAsync(winsdkfb::FBResult loginResult);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryGetAppPermissionsAfterLoginAsync(winsdkfb::FBResult loginResult);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> RunOAuthOnUiThreadAsync(winrt::Windows::Foundation::Collections::PropertySet Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> RunWebViewLoginOnUIThreadAsync(winrt::Windows::Foundation::Collections::PropertySet Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebViewAsync(winrt::Windows::Foundation::Collections::PropertySet Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebAuthBrokerAsync(winrt::Windows::Foundation::Collections::PropertySet Parameters);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginSilentlyAsync(winrt::Windows::Foundation::Collections::PropertySet Parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		hstring GetWebAccountProviderRedirectUriString();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FBPermissions Permissions);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebAccountProviderAsync(winsdkfb::FBPermissions Permissions);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FBPermissions Permissions);
		winsdkfb::FBResult ExtractAccessTokenDataFromResponseData(winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData);
		winsdkfb::FBResult FBResultFromTokenRequestResult(winrt::Windows::Security::Authentication::Web::Core::WebTokenRequestResult RequestResult);
#endif

		bool _loggedIn;
		int _APIMajorVersion;
		int _APIMinorVersion;
		std::wstring _FBAppId;
		std::wstring _WinAppId;
		std::wstring _AppResponse;
		std::wstring _webViewRedirectDomain;
		std::wstring _webViewRedirectPath;
		winsdkfb::FBAccessTokenData _AccessTokenData{ nullptr };
		winsdkfb::Graph::FBUser _user{ nullptr };
		winsdkfb::FBDialog _dialog{ nullptr };
		winsdkfb::FBResult _asyncResult{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBSession : FBSessionT<FBSession, implementation::FBSession>
	{
	};
}
