#pragma once

#include "FacebookProfilePictureControl.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookProfilePictureControl : FacebookProfilePictureControlT<FacebookProfilePictureControl>
	{
		FacebookProfilePictureControl();

		hstring UserId();
		void UserId(hstring const& value);

		FacebookSDK::CroppingType CropMode();
		void CropMode(FacebookSDK::CroppingType const& value);

	private:
		static winrt::Windows::UI::Xaml::DependencyProperty _userIdProperty;

		static void UserIdPropertyChanged(
			winrt::Windows::UI::Xaml::DependencyObject const& d,
			winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e
		);

		fire_and_forget SetImageSourceFromUserId();

		void SetImageSourceFromResource();

		void Update();

		winrt::Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult>
			GetProfilePictureInfoAsync(int width, int height);

		CroppingType _cropMode;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookProfilePictureControl : FacebookProfilePictureControlT<FacebookProfilePictureControl, implementation::FacebookProfilePictureControl>
	{
	};
}
