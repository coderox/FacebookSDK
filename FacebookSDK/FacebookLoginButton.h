#pragma once

#include "FacebookLoginButton.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton>
	{
		FacebookLoginButton()
		{
			DefaultStyleKey(winrt::box_value(L"FacebookSDK.FacebookLoginButton"));
		};

		void OnApplyTemplate();

		FacebookSDK::FacebookPermissions Permissions();
		void Permissions(FacebookSDK::FacebookPermissions const& value);

		void InitWithPermissions(FacebookSDK::FacebookPermissions const& permissions);

		winrt::event_token FacebookLoginError(FacebookSDK::FacebookLoginErrorHandler const& handler);
		void FacebookLoginError(winrt::event_token const& token) noexcept;

		winrt::event_token FetchedUserInfo(FacebookSDK::FetchedUserInfoHandler const& handler);
		void FetchedUserInfo(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedInUser(FacebookSDK::ShowingLoggedInUserHandler const& handler);
		void ShowingLoggedInUser(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedOutUser(FacebookSDK::ShowingLoggedOutUserHandler const& handler);
		void ShowingLoggedOutUser(winrt::event_token const& token) noexcept;

		winrt::fire_and_forget OnClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

	private:
		winrt::hstring GetPermissions();

		winrt::event<FacebookLoginErrorHandler> _facebookLoginError;
		winrt::event<FetchedUserInfoHandler> _fetchedUserInfo;
		winrt::event<ShowingLoggedInUserHandler> _showingLoggedInUser;
		winrt::event<ShowingLoggedOutUserHandler> _showingLoggedOutUser;
		FacebookSDK::FacebookPermissions _permissions{ nullptr };
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton, implementation::FacebookLoginButton>
	{
	};
}
