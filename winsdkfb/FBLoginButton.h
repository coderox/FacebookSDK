#pragma once

#include "FBLoginButton.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBLoginButton : FBLoginButtonT<FBLoginButton>
	{
		FBLoginButton()
		{
			DefaultStyleKey(winrt::box_value(L"winsdkfb.FBLoginButton"));
		};

		void OnApplyTemplate();

		winsdkfb::FBPermissions Permissions();
		void Permissions(winsdkfb::FBPermissions const& value);

		void InitWithPermissions(winsdkfb::FBPermissions const& permissions);

		winrt::event_token FBLoginError(winsdkfb::FBLoginErrorHandler const& handler);
		void FBLoginError(winrt::event_token const& token) noexcept;

		winrt::event_token FetchedUserInfo(winsdkfb::FetchedUserInfoHandler const& handler);
		void FetchedUserInfo(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedInUser(winsdkfb::ShowingLoggedInUserHandler const& handler);
		void ShowingLoggedInUser(winrt::event_token const& token) noexcept;

		winrt::event_token ShowingLoggedOutUser(winsdkfb::ShowingLoggedOutUserHandler const& handler);
		void ShowingLoggedOutUser(winrt::event_token const& token) noexcept;

		winrt::fire_and_forget OnClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

	private:
		winrt::hstring GetPermissions();

		winrt::event<FBLoginErrorHandler> _facebookLoginError;
		winrt::event<FetchedUserInfoHandler> _fetchedUserInfo;
		winrt::event<ShowingLoggedInUserHandler> _showingLoggedInUser;
		winrt::event<ShowingLoggedOutUserHandler> _showingLoggedOutUser;
		winsdkfb::FBPermissions _permissions{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBLoginButton : FBLoginButtonT<FBLoginButton, implementation::FBLoginButton>
	{
	};
}
