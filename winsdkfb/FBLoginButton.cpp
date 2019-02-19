#include "pch.h"
#include "FBLoginButton.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

namespace winrt::winsdkfb::implementation
{
	void FBLoginButton::OnApplyTemplate() {
		Click({ this, &FBLoginButton::OnClick });
	}

	winsdkfb::FBPermissions FBLoginButton::Permissions()
	{
		return _permissions;
	}

	void FBLoginButton::Permissions(winsdkfb::FBPermissions const& value)
	{
		_permissions = value;
	}

	void FBLoginButton::InitWithPermissions(winsdkfb::FBPermissions const& permissions)
	{
		_permissions = permissions;
	}

	event_token FBLoginButton::FBLoginError(winsdkfb::FBLoginErrorHandler const& handler)
	{
		return _facebookLoginError.add(handler);
	}

	void FBLoginButton::FBLoginError(event_token const& token) noexcept
	{
		_facebookLoginError.remove(token);
	}

	event_token FBLoginButton::FetchedUserInfo(winsdkfb::FetchedUserInfoHandler const& handler)
	{
		return _fetchedUserInfo.add(handler);
	}

	void FBLoginButton::FetchedUserInfo(event_token const& token) noexcept
	{
		_fetchedUserInfo.remove(token);
	}

	event_token FBLoginButton::ShowingLoggedInUser(winsdkfb::ShowingLoggedInUserHandler const& handler)
	{
		return _showingLoggedInUser.add(handler);
	}

	void FBLoginButton::ShowingLoggedInUser(event_token const& token) noexcept
	{
		_showingLoggedInUser.remove(token);
	}

	event_token FBLoginButton::ShowingLoggedOutUser(winsdkfb::ShowingLoggedOutUserHandler const& handler)
	{
		return _showingLoggedOutUser.add(handler);
	}

	void FBLoginButton::ShowingLoggedOutUser(event_token const& token) noexcept
	{
		_showingLoggedOutUser.remove(token);
	}

	fire_and_forget FBLoginButton::OnClick(IInspectable const& sender, RoutedEventArgs const& e)
	{
		auto session = FBSession::ActiveSession();

		if (!session.LoggedIn()) {
			auto result = co_await session.LoginAsync(_permissions);
			if (result.Succeeded()) {
				auto user = result.Object().try_as<Graph::FBUser>();
				_fetchedUserInfo(*this, user);
			}
		}
	}

	hstring FBLoginButton::GetPermissions() {
		return _permissions != nullptr ? _permissions.ToString() : L"";
	}
}
