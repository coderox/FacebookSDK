#pragma once

#include "FBSession.g.h"
#include <mutex>

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

		Windows::Foundation::IAsyncAction LogoutAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions, winsdkfb::SessionLoginBehavior behavior);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryRefreshAccessTokenAsync();
        Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ReauthorizeAsync(winsdkfb::FBPermissions permissions);

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(hstring const& domain, hstring const& Path);

		static winsdkfb::FBSession ActiveSession();
		static Windows::Storage::ApplicationDataContainer DataContainer();

	private:
		void ParseOAuthResponse(Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		Windows::Foundation::Uri BuildLoginUri(Windows::Foundation::Collections::PropertySet parameters);

		BOOL IsRerequest(Windows::Foundation::Collections::PropertySet Parameters);
		BOOL IsReauthorize(Windows::Foundation::Collections::PropertySet Parameters);

		hstring GetGrantedPermissions();
		hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		winsdkfb::FBResult ProcessAuthResult(Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetUserInfoAsync(winsdkfb::FBAccessTokenData const& tokenData);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CheckForExistingTokenAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetAppPermissionsAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const& Parameters);

		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryGetUserInfoAfterLoginAsync(winsdkfb::FBResult loginResult);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryGetAppPermissionsAfterLoginAsync(winsdkfb::FBResult loginResult);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> RunOAuthOnUiThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> RunWebViewLoginOnUIThreadAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebViewAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebAuthBrokerAsync(Windows::Foundation::Collections::PropertySet Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginSilentlyAsync(Windows::Foundation::Collections::PropertySet Parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		hstring GetWebAccountProviderRedirectUriString();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FBPermissions Permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebAccountProviderAsync(winsdkfb::FBPermissions Permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FBPermissions Permissions);
		winsdkfb::FBResult ExtractAccessTokenDataFromResponseData(Windows::Foundation::Collections::IVectorView<Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData);
		winsdkfb::FBResult FBResultFromTokenRequestResult(Windows::Security::Authentication::Web::Core::WebTokenRequestResult RequestResult);
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
		std::mutex _fileMutex;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBSession : FBSessionT<FBSession, implementation::FBSession>
	{
	};
}
