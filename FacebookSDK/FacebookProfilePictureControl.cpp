﻿#include "pch.h"
#include "FacebookProfilePictureControl.h"
#include "FacebookSingleValue.h"
#include <string>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Core;

#define INT_STRING_LENGTH 16
#define PROFILE_PICTURE_SILLHOUETTE_IMAGE L"ms-appx:///FacebookSDK/Images/fb_blank_profile_portrait.png"

namespace winrt::FacebookSDK::implementation
{
	DependencyProperty FacebookProfilePictureControl::_userIdProperty = DependencyProperty::Register(
		L"UserId",
		TypeName(xaml_typename<hstring>()),
		TypeName(xaml_typename<FacebookSDK::FacebookProfilePictureControl>()),
		PropertyMetadata(box_value(hstring(L"")), PropertyChangedCallback(&FacebookProfilePictureControl::UserIdPropertyChanged))
	);

	FacebookProfilePictureControl::FacebookProfilePictureControl()
		: _cropMode(CroppingType::Square)
	{
		InitializeComponent();
		Update();
	}

	hstring FacebookProfilePictureControl::UserId()
	{
		return unbox_value<hstring>(GetValue(_userIdProperty));
	}

	void FacebookProfilePictureControl::UserId(hstring const& value)
	{
		SetValue(_userIdProperty, box_value(value));
		Update();
	}

	FacebookSDK::CroppingType FacebookProfilePictureControl::CropMode()
	{
		return _cropMode;
	}

	void FacebookProfilePictureControl::CropMode(FacebookSDK::CroppingType const& value)
	{
		if (_cropMode != value) {
			_cropMode = value;
			Update();
		}
	}

	void FacebookProfilePictureControl::UserIdPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto instance = d.as<FacebookSDK::FacebookProfilePictureControl>();
		instance.UserId(unbox_value<hstring>(e.NewValue()));
	}

	fire_and_forget FacebookProfilePictureControl::SetImageSourceFromUserId()
	{
		int height = -1;

		// specify height == width for square.  If user wants original aspect ratio,
		// only specify width, and FB graph API will scale and preserve ratio.
		if (CropMode() == CroppingType::Square)
		{
			height = (int)Width();
		}

		auto result = co_await GetProfilePictureInfoAsync(Width(), height);
		if (result != nullptr && result.Succeeded()) {
			Graph::FBProfilePictureData info{ nullptr };
			info = result.Object().as<FacebookSDK::Graph::FBProfilePictureData>();
			auto coreWindow = CoreApplication::MainView().CoreWindow();
			if (coreWindow) {
				coreWindow.Dispatcher().RunAsync(CoreDispatcherPriority::Low,
					DispatchedHandler([info, this]() {
					if (info && info.Data() && !info.Data().Url().empty()) {
						profilePic().Stretch(Stretch::Uniform);
						profilePic().Source(BitmapImage(Uri(info.Data().Url())));
					}
				}));
			}
		}
	}

	void FacebookProfilePictureControl::SetImageSourceFromResource()
	{
		auto coreWindow = CoreApplication::MainView().CoreWindow();
		if (coreWindow) {
			coreWindow.Dispatcher().RunAsync(CoreDispatcherPriority::Low,
				DispatchedHandler([=]() {
				Uri uri(PROFILE_PICTURE_SILLHOUETTE_IMAGE);
				profilePic().Stretch(Stretch::Uniform);
				profilePic().Source(BitmapImage(Uri(uri)));

			}));
		}
	}

	void FacebookProfilePictureControl::Update()
	{
		if (UserId().empty()) {
			SetImageSourceFromResource();
		}
		else {
			SetImageSourceFromUserId();
		}
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookProfilePictureControl::GetProfilePictureInfoAsync(int width, int height) {
		PropertySet parameters;
		wchar_t whBuffer[INT_STRING_LENGTH];
		FacebookSDK::FacebookResult result;

		parameters.Insert(L"redirect", box_value(L"false"));

		if (width > -1) {
			if (!_itow_s(width, whBuffer, INT_STRING_LENGTH, 10))
			{
				hstring value(whBuffer);
				parameters.Insert(L"width", box_value(value));
			}
		}
		if (height > -1) {
			if (!_itow_s(height, whBuffer, INT_STRING_LENGTH, 10))
			{
				hstring value(whBuffer);
				parameters.Insert(L"height", box_value(value));
			}
		}

		if (!UserId().empty()) {
			hstring path(UserId() + L"/picture");

			auto objectFactory = JsonClassFactory([](hstring jsonText) -> IInspectable
			{
				return Graph::FBProfilePictureData::FromJson(jsonText);
			});

			FacebookSDK::FacebookSingleValue value = make<FacebookSingleValue>(
				path,
				parameters,
				objectFactory);
			result = co_await value.GetAsync();
		}

		co_return result;
	}
}
