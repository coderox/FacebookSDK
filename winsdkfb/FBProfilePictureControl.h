#pragma once

#include "FBProfilePictureControl.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBProfilePictureControl : FBProfilePictureControlT<FBProfilePictureControl>
	{
		FBProfilePictureControl();

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

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult>
			GetProfilePictureInfoAsync(int width, int height);

		CroppingType _cropMode;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBProfilePictureControl : FBProfilePictureControlT<FBProfilePictureControl, implementation::FBProfilePictureControl>
	{
	};
}
