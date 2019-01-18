#pragma once

#include "FacebookSession.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookSession : FacebookSessionT<FacebookSession>
	{
		FacebookSession() = default;

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
		void SetApiVersion(int32_t major, int32_t minor);
		void SetWebViewRedirectUrl(hstring const& domain, hstring const& Path);

		static FacebookSDK::FacebookSession ActiveSession();
		static Windows::Storage::ApplicationDataContainer DataContainer();

		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> TryRefreshAccessTokenAsync();
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookSession : FacebookSessionT<FacebookSession, implementation::FacebookSession>
	{
	};
}
