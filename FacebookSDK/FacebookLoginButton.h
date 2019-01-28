#pragma once

#include "FacebookLoginButton.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton>
    {
		FacebookLoginButton() = default;

		void OnApplyTemplate() const;

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

		void OnClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

	private:
		winrt::hstring GetPermissions();
				
		FacebookSDK::FacebookPermissions _permissions{ nullptr };
    };
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookLoginButton : FacebookLoginButtonT<FacebookLoginButton, implementation::FacebookLoginButton>
	{
	};
}
