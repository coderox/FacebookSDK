#pragma once

#include "FacebookProfilePictureControl.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookProfilePictureControl : FacebookProfilePictureControlT<FacebookProfilePictureControl>
	{
		FacebookProfilePictureControl();

		hstring UserId();
		void UserId(hstring const& value);

		winsdkfb::CroppingType CropMode();
		void CropMode(winsdkfb::CroppingType const& value);

	private:
		static winrt::Windows::UI::Xaml::DependencyProperty _userIdProperty;

		static void UserIdPropertyChanged(
			winrt::Windows::UI::Xaml::DependencyObject const& d,
			winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e
		);

		fire_and_forget SetImageSourceFromUserId();

		void SetImageSourceFromResource();

		void Update();

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult>
			GetProfilePictureInfoAsync(int width, int height);

		CroppingType _cropMode;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookProfilePictureControl : FacebookProfilePictureControlT<FacebookProfilePictureControl, implementation::FacebookProfilePictureControl>
	{
	};
}
