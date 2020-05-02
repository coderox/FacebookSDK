#pragma once

#include "FBAccessTokenData.h"
#include "Graph/FBUser.h"
#include "FBDialog.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Authentication.Web.Core.h>
#include <winrt/Windows.Storage.h>

namespace winsdkfb
{
	struct FBSession
	{
		FBSession();
		~FBSession();

		winrt::hstring FBAppId();
		void FBAppId(winrt::hstring const& value);

		winrt::hstring WinAppId();
		void WinAppId(winrt::hstring const& value);

		winrt::hstring AppResponse();

		bool LoggedIn();

		winsdkfb::FBAccessTokenData AccessTokenData();
		void AccessTokenData(winsdkfb::FBAccessTokenData const& value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		winsdkfb::Graph::FBUser User();

		winrt::hstring WebViewRedirectDomain();
		winrt::hstring WebViewRedirectPath();

		Windows::Foundation::IAsyncAction LogoutAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowFeedDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowRequestsDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowSendDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions, winsdkfb::SessionLoginBehavior behavior);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryRefreshAccessTokenAsync();

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(winrt::hstring const& domain, winrt::hstring const& Path);

		static winsdkfb::FBSession ActiveSession();
		static winrt::Windows::Storage::ApplicationDataContainer DataContainer();

	private:
		void ParseOAuthResponse(winrt::Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		winrt::Windows::Foundation::Uri BuildLoginUri(winrt::Windows::Foundation::Collections::PropertySet parameters);

		bool IsRerequest(Windows::Foundation::Collections::PropertySet Parameters);

		winrt::hstring GetGrantedPermissions();
		winrt::hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		winsdkfb::FBResult ProcessAuthResult(winrt::Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

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
		winrt::hstring GetWebAccountProviderRedirectUriString();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FBPermissions Permissions);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> TryLoginViaWebAccountProviderAsync(winsdkfb::FBPermissions Permissions);
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FBPermissions Permissions);
		winsdkfb::FBResult ExtractAccessTokenDataFromResponseData(winrt::Windows::Foundation::Collections::IVectorView<Windows::Security::Authentication::Web::Core::WebTokenResponse> ResponseData);
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

