#include "pch.h"
#include "FacebookSession.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::FacebookSDK::implementation
{
	hstring FacebookSession::FacebookAppId()
	{
		throw hresult_not_implemented();
	}

	void FacebookSession::FacebookAppId(hstring const& value)
	{
		throw hresult_not_implemented();
	}

	hstring FacebookSession::WinAppId()
	{
		throw hresult_not_implemented();
	}

	void FacebookSession::WinAppId(hstring const& value)
	{
		throw hresult_not_implemented();
	}

	hstring FacebookSession::AppResponse()
	{
		throw hresult_not_implemented();
	}

	bool FacebookSession::LoggedIn()
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookAccessTokenData FacebookSession::AccessTokenData()
	{
		throw hresult_not_implemented();
	}

	void FacebookSession::AccessTokenData(FacebookSDK::FacebookAccessTokenData const& value)
	{
		throw hresult_not_implemented();
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
		throw hresult_not_implemented();
	}

	hstring FacebookSession::WebViewRedirectDomain()
	{
		throw hresult_not_implemented();
	}

	hstring FacebookSession::WebViewRedirectPath()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncAction FacebookSession::LogoutAsync()
	{
		throw hresult_not_implemented();
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
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookSession::TryRefreshAccessTokenAsync()
	{
		throw hresult_not_implemented();
	}
}
