#pragma once

#include "FBAccessTokenData.h"
#include "Graph/FBUser.h"
#include "FBDialog.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Authentication.Web.Core.h>
#include <winrt/Windows.Storage.h>
#include <pplawait.h>

namespace winsdkfb
{
	enum SessionDefaultAudience {
		None = 0,
		OnlyMe = 10,
		Friends = 20,
		Everyone = 30
	};

	enum SessionLoginBehavior {
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

		std::shared_ptr<winsdkfb::FBAccessTokenData> AccessTokenData();
		void AccessTokenData(std::shared_ptr<winsdkfb::FBAccessTokenData> value);

		int32_t APIMajorVersion();
		int32_t APIMinorVersion();

		std::shared_ptr<winsdkfb::Graph::FBUser> User();

		winrt::hstring WebViewRedirectDomain();
		winrt::hstring WebViewRedirectPath();

		winrt::Windows::Foundation::IAsyncAction LogoutAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> ShowFeedDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> ShowRequestsDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> ShowSendDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> LoginAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> LoginAsync(std::shared_ptr<winsdkfb::FBPermissions> permissions);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> LoginAsync(std::shared_ptr<winsdkfb::FBPermissions> permissions, winsdkfb::SessionLoginBehavior behavior);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryRefreshAccessTokenAsync();

		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(winrt::hstring const& domain, winrt::hstring const& Path);

		static winsdkfb::FBSession* ActiveSession();
		static winrt::Windows::Storage::ApplicationDataContainer DataContainer();

	private:
		void ParseOAuthResponse(winrt::Windows::Foundation::Uri ResponseUri);
		void SaveGrantedPermissions();
		winrt::Windows::Foundation::Uri BuildLoginUri(winrt::Windows::Foundation::Collections::PropertySet parameters);

		bool IsRerequest(winrt::Windows::Foundation::Collections::PropertySet const& Parameters);

		winrt::hstring GetGrantedPermissions();
		winrt::hstring GetWebAuthRedirectUriString();
		winrt::fire_and_forget TrySaveTokenData();
		winrt::Windows::Foundation::IAsyncAction TryDeleteTokenDataAsync();
		std::shared_ptr<winsdkfb::FBResult> ProcessAuthResult(winrt::Windows::Security::Authentication::Web::WebAuthenticationResult authResult);

		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> GetUserInfoAsync(winsdkfb::FBAccessTokenData const tokenData);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> CheckForExistingTokenAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> GetAppPermissionsAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> ShowLoginDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);

		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryGetUserInfoAfterLoginAsync(std::shared_ptr<winsdkfb::FBResult> loginResult);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryGetAppPermissionsAfterLoginAsync(std::shared_ptr<winsdkfb::FBResult> loginResult);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> RunOAuthOnUiThreadAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> RunWebViewLoginOnUIThreadAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryLoginViaWebViewAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryLoginViaWebAuthBrokerAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryLoginSilentlyAsync(winrt::Windows::Foundation::Collections::PropertySet const Parameters);

#if defined(_WIN32_WINNT_WIN10) && (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		winrt::hstring GetWebAccountProviderRedirectUriString();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> CheckWebAccountProviderForExistingTokenAsync(winsdkfb::FBPermissions Permissions);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> TryLoginViaWebAccountProviderAsync(winsdkfb::FBPermissions Permissions);
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> CallWebAccountProviderOnUiThreadAsync(winsdkfb::FBPermissions Permissions);
		std::shared_ptr<winsdkfb::FBResult> ExtractAccessTokenDataFromResponseData(winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Security::Authentication::Web::Core::WebTokenResponse> const ResponseData);
		std::shared_ptr<winsdkfb::FBResult> FBResultFromTokenRequestResult(winrt::Windows::Security::Authentication::Web::Core::WebTokenRequestResult const RequestResult);
#endif

		bool _loggedIn;
		int _APIMajorVersion;
		int _APIMinorVersion;
		std::wstring _FBAppId;
		std::wstring _WinAppId;
		std::wstring _AppResponse;
		std::wstring _webViewRedirectDomain;
		std::wstring _webViewRedirectPath;
		std::shared_ptr<winsdkfb::FBAccessTokenData> _AccessTokenData{ nullptr };
		std::shared_ptr<winsdkfb::Graph::FBUser> _user{ nullptr };
		std::shared_ptr<winsdkfb::FBDialog> _dialog{ nullptr };
		std::shared_ptr<winsdkfb::FBResult> _asyncResult{ nullptr };
	};
}

