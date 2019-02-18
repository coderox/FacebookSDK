#include "pch.h"
#include "FacebookLoginButton.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

namespace winrt::winsdkfb::implementation
{
	void FacebookLoginButton::OnApplyTemplate() {
		Click({ this, &FacebookLoginButton::OnClick });
	}

	winsdkfb::FacebookPermissions FacebookLoginButton::Permissions()
	{
		return _permissions;
	}

	void FacebookLoginButton::Permissions(winsdkfb::FacebookPermissions const& value)
	{
		_permissions = value;
	}

	void FacebookLoginButton::InitWithPermissions(winsdkfb::FacebookPermissions const& permissions)
	{
		_permissions = permissions;
	}

	event_token FacebookLoginButton::FacebookLoginError(winsdkfb::FacebookLoginErrorHandler const& handler)
	{
		return _facebookLoginError.add(handler);
	}

	void FacebookLoginButton::FacebookLoginError(event_token const& token) noexcept
	{
		_facebookLoginError.remove(token);
	}

	event_token FacebookLoginButton::FetchedUserInfo(winsdkfb::FetchedUserInfoHandler const& handler)
	{
		return _fetchedUserInfo.add(handler);
	}

	void FacebookLoginButton::FetchedUserInfo(event_token const& token) noexcept
	{
		_fetchedUserInfo.remove(token);
	}

	event_token FacebookLoginButton::ShowingLoggedInUser(winsdkfb::ShowingLoggedInUserHandler const& handler)
	{
		return _showingLoggedInUser.add(handler);
	}

	void FacebookLoginButton::ShowingLoggedInUser(event_token const& token) noexcept
	{
		_showingLoggedInUser.remove(token);
	}

	event_token FacebookLoginButton::ShowingLoggedOutUser(winsdkfb::ShowingLoggedOutUserHandler const& handler)
	{
		return _showingLoggedOutUser.add(handler);
	}

	void FacebookLoginButton::ShowingLoggedOutUser(event_token const& token) noexcept
	{
		_showingLoggedOutUser.remove(token);
	}

	fire_and_forget FacebookLoginButton::OnClick(IInspectable const& sender, RoutedEventArgs const& e)
	{
		auto session = FacebookSession::ActiveSession();

		if (!session.LoggedIn()) {
			auto result = co_await session.LoginAsync(_permissions);
			if (result.Succeeded()) {
				auto user = result.Object().try_as<Graph::FBUser>();
				_fetchedUserInfo(*this, user);
			}
		}
	}

	hstring FacebookLoginButton::GetPermissions() {
		return _permissions != nullptr ? _permissions.ToString() : L"";
	}
}
