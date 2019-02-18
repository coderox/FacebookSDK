#pragma once

#include "FacebookLoginButton.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton>
	{
		FacebookLoginButton()
		{
			DefaultStyleKey(winrt::box_value(L"winsdkfb.FacebookLoginButton"));
		};

		void OnApplyTemplate();

		winsdkfb::FacebookPermissions Permissions();
		void Permissions(winsdkfb::FacebookPermissions const& value);

		void InitWithPermissions(winsdkfb::FacebookPermissions const& permissions);

		winrt::event_token FacebookLoginError(winsdkfb::FacebookLoginErrorHandler const& handler);
		void FacebookLoginError(winrt::event_token const& token) noexcept;

		winrt::event_token FetchedUserInfo(winsdkfb::FetchedUserInfoHandler const& handler);
		void FetchedUserInfo(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedInUser(winsdkfb::ShowingLoggedInUserHandler const& handler);
		void ShowingLoggedInUser(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedOutUser(winsdkfb::ShowingLoggedOutUserHandler const& handler);
		void ShowingLoggedOutUser(winrt::event_token const& token) noexcept;

		winrt::fire_and_forget OnClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

	private:
		winrt::hstring GetPermissions();

		winrt::event<FacebookLoginErrorHandler> _facebookLoginError;
		winrt::event<FetchedUserInfoHandler> _fetchedUserInfo;
		winrt::event<ShowingLoggedInUserHandler> _showingLoggedInUser;
		winrt::event<ShowingLoggedOutUserHandler> _showingLoggedOutUser;
		winsdkfb::FacebookPermissions _permissions{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton, implementation::FacebookLoginButton>
	{
	};
}
