#include "pch.h"
#include "FacebookLoginButton.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

namespace winrt::FacebookSDK::implementation
{
	void FacebookLoginButton::OnApplyTemplate() {
		Click({ this, &FacebookLoginButton::OnClick });
	}

	FacebookSDK::FacebookPermissions FacebookLoginButton::Permissions()
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::Permissions(FacebookSDK::FacebookPermissions const& value)
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::InitWithPermissions(FacebookSDK::FacebookPermissions const& permissions)
	{
		throw hresult_not_implemented();
	}

	event_token FacebookLoginButton::FacebookLoginError(FacebookSDK::FacebookLoginErrorHandler const& handler)
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::FacebookLoginError(event_token const& token) noexcept
	{
		throw hresult_not_implemented();
	}

	event_token FacebookLoginButton::FetchedUserInfo(FacebookSDK::FetchedUserInfoHandler const& handler)
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::FetchedUserInfo(event_token const& token) noexcept
	{
		throw hresult_not_implemented();
	}

	event_token FacebookLoginButton::ShowingLoggedInUser(FacebookSDK::ShowingLoggedInUserHandler const& handler)
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::ShowingLoggedInUser(event_token const& token) noexcept
	{
		throw hresult_not_implemented();
	}

	event_token FacebookLoginButton::ShowingLoggedOutUser(FacebookSDK::ShowingLoggedOutUserHandler const& handler)
	{
		throw hresult_not_implemented();
	}

	void FacebookLoginButton::ShowingLoggedOutUser(event_token const& token) noexcept
	{
		throw hresult_not_implemented();
	}

	fire_and_forget FacebookLoginButton::OnClick(IInspectable const& sender, RoutedEventArgs const& e) {
		co_await winrt::resume_background();
		co_return;
	}
	
	hstring FacebookLoginButton::GetPermissions() {
		throw hresult_not_implemented();
	}
}
