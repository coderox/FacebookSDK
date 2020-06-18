#pragma once

#include "FBAccessTokenData.h"
#include "Graph/FBUser.h"
#include "FBDialog.h"
#include <map>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Authentication.Web.Core.h>
#include <winrt/Windows.Storage.h>
#include <pplawait.h>

namespace winsdkfb
{
	enum class SessionDefaultAudience {
		None = 0,
		OnlyMe = 10,
		Friends = 20,
		Everyone = 30
	};

	enum class SessionLoginBehavior {
		WebView,
		WebAuth,
		WebAccountProvider,
		DefaultOrdering,
		Silent
	};

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
		void AccessTokenData(winsdkfb::FBAccessTokenData value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		winsdkfb::Graph::FBUser User();

		winrt::hstring WebViewRedirectDomain();
		winrt::hstring WebViewRedirectPath();

		winrt::Windows::Foundation::IAsyncAction LogoutAsync();
		concurrency::task<winsdkfb::FBResult> ShowFeedDialogAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> ShowRequestsDialogAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> ShowSendDialogAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> LoginAsync();
		concurrency::task<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions);
		concurrency::task<winsdkfb::FBResult> LoginAsync(winsdkfb::FBPermissions permissions, winsdkfb::SessionLoginBehavior behavior);
		concurrency::task<winsdkfb::FBResult> TryRefreshAccessTokenAsync();
		concurrency::task<winsdkfb::FBResult> ReauthorizeAsync(winsdkfb::FBPermissions permissions);

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(winrt::hstring const& domain, winrt::hstring const& path);

		static std::shared_ptr<winsdkfb::FBSession> ActiveSession();
		static winrt::Windows::Storage::ApplicationDataContainer DataContainer();

	private:
		void ParseOAuthResponse(winrt::Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		winrt::Windows::Foundation::Uri BuildLoginUri(std::unordered_map<winrt::hstring, winrt::hstring> parameters);

		bool IsRerequest(std::unordered_map<winrt::hstring, winrt::hstring> Parameters);

		winrt::hstring GetGrantedPermissions();
		winrt::hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		winrt::Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		winsdkfb::FBResult ProcessAuthResult(winrt::Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		concurrency::task<winsdkfb::FBResult> GetUserInfoAsync(winsdkfb::FBAccessTokenData const tokenData);
		concurrency::task<winsdkfb::FBResult> CheckForExistingTokenAsync();
		concurrency::task<winsdkfb::FBResult> GetAppPermissionsAsync();
		concurrency::task<winsdkfb::FBResult> ShowLoginDialogAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);

		concurrency::task<winsdkfb::FBResult> TryGetUserInfoAfterLoginAsync(winsdkfb::FBResult loginResult);
		concurrency::task<winsdkfb::FBResult> TryGetAppPermissionsAfterLoginAsync(winsdkfb::FBResult loginResult);
		concurrency::task<winsdkfb::FBResult> RunOAuthOnUiThreadAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> RunWebViewLoginOnUIThreadAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> TryLoginViaWebViewAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> TryLoginViaWebAuthBrokerAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		concurrency::task<winsdkfb::FBResult> TryLoginSilentlyAsync(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		winrt::hstring GetWebAccountProviderRedirectUriString();
		concurrency::task<winsdkfb::FBResult> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FBPermissions Permissions);
		concurrency::task<winsdkfb::FBResult> TryLoginViaWebAccountProviderAsync(winsdkfb::FBPermissions Permissions);
		concurrency::task<winsdkfb::FBResult> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FBPermissions Permissions);
		winsdkfb::FBResult ExtractAccessTokenDataFromResponseData(winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Security::Authentication::Web::Core::WebTokenResponse> const ResponseData);
		winsdkfb::FBResult FBResultFromTokenRequestResult(winrt::Windows::Security::Authentication::Web::Core::WebTokenRequestResult const RequestResult);
#endif

		bool _loggedIn;
		int _APIMajorVersion;
		int _APIMinorVersion;
		std::wstring _FBAppId;
		std::wstring _WinAppId;
		std::wstring _AppResponse;
		std::wstring _webViewRedirectDomain;
		std::wstring _webViewRedirectPath;
		winsdkfb::FBAccessTokenData _AccessTokenData;
		winsdkfb::Graph::FBUser _user;
		winsdkfb::FBResult _asyncResult;
	};
}

