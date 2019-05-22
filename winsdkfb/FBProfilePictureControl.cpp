#include "pch.h"
#include "FBProfilePictureControl.h"
#include "FBSingleValue.h"
#include <string>
#if __has_include("FBProfilePictureControl.g.cpp")
#include "FBProfilePictureControl.g.cpp"
#endif

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
#define PROFILE_PICTURE_SILLHOUETTE_IMAGE L"ms-appx:///winsdkfb/Images/fb_blank_profile_portrait.png"

namespace winrt::winsdkfb::implementation
{
	//DependencyProperty FBProfilePictureControl::_userIdProperty = DependencyProperty::Register(
	//	L"UserId",
	//	TypeName(xaml_typename<hstring>()),
	//	TypeName(xaml_typename<winsdkfb::FBProfilePictureControl>()),
	//	PropertyMetadata(box_value(hstring(L"")), PropertyChangedCallback(&FBProfilePictureControl::UserIdPropertyChanged))
	//);

	FBProfilePictureControl::FBProfilePictureControl()
		//: _cropMode(CroppingType::Square)
	{
		InitializeComponent();
		//Update();
	}

	//hstring FBProfilePictureControl::UserId()
	//{
	//	return unbox_value<hstring>(GetValue(_userIdProperty));
	//}

	//void FBProfilePictureControl::UserId(hstring const& value)
	//{
	//	SetValue(_userIdProperty, box_value(value));
	//	Update();
	//}

	//winsdkfb::CroppingType FBProfilePictureControl::CropMode()
	//{
	//	return _cropMode;
	//}

	//void FBProfilePictureControl::CropMode(winsdkfb::CroppingType const& value)
	//{
	//	if (_cropMode != value) {
	//		_cropMode = value;
	//		Update();
	//	}
	//}

	//void FBProfilePictureControl::UserIdPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	//{
	//	auto instance = d.as<winsdkfb::FBProfilePictureControl>();
	//	instance.UserId(unbox_value<hstring>(e.NewValue()));
	//}

	//fire_and_forget FBProfilePictureControl::SetImageSourceFromUserId()
	//{
	//	int height = -1;

	//	// specify height == width for square.  If user wants original aspect ratio,
	//	// only specify width, and FB graph API will scale and preserve ratio.
	//	if (CropMode() == CroppingType::Square)
	//	{
	//		height = (int)Width();
	//	}

	//	auto result = co_await GetProfilePictureInfoAsync((int)Width(), height);
	//	if (result != nullptr && result.Succeeded()) {
	//		Graph::FBProfilePictureData info{ nullptr };
	//		info = result.Object().as<winsdkfb::Graph::FBProfilePictureData>();
	//		auto coreWindow = CoreApplication::MainView().CoreWindow();
	//		if (coreWindow) {
	//			coreWindow.Dispatcher().RunAsync(CoreDispatcherPriority::Low,
	//				DispatchedHandler([info, this]() {
	//				if (info && info.Data() && !info.Data().Url().empty()) {
	//					profilePic().Stretch(Stretch::Uniform);
	//					profilePic().Source(BitmapImage(Uri(info.Data().Url())));
	//				}
	//			}));
	//		}
	//	}
	//}

	//void FBProfilePictureControl::SetImageSourceFromResource()
	//{
	//	auto coreWindow = CoreApplication::MainView().CoreWindow();
	//	if (coreWindow) {
	//		coreWindow.Dispatcher().RunAsync(CoreDispatcherPriority::Low,
	//			DispatchedHandler([=]() {
	//			Uri uri(PROFILE_PICTURE_SILLHOUETTE_IMAGE);
	//			profilePic().Stretch(Stretch::Uniform);
	//			profilePic().Source(BitmapImage(Uri(uri)));

	//		}));
	//	}
	//}

	//void FBProfilePictureControl::Update()
	//{
	//	if (UserId().empty()) {
	//		SetImageSourceFromResource();
	//	}
	//	else {
	//		SetImageSourceFromUserId();
	//	}
	//}

	//IAsyncOperation<winsdkfb::FBResult> FBProfilePictureControl::GetProfilePictureInfoAsync(int width, int height) {
	//	PropertySet parameters;
	//	wchar_t whBuffer[INT_STRING_LENGTH];
	//	winsdkfb::FBResult result;

	//	parameters.Insert(L"redirect", box_value(L"false"));

	//	if (width > -1) {
	//		if (!_itow_s(width, whBuffer, INT_STRING_LENGTH, 10))
	//		{
	//			hstring value(whBuffer);
	//			parameters.Insert(L"width", box_value(value));
	//		}
	//	}
	//	if (height > -1) {
	//		if (!_itow_s(height, whBuffer, INT_STRING_LENGTH, 10))
	//		{
	//			hstring value(whBuffer);
	//			parameters.Insert(L"height", box_value(value));
	//		}
	//	}

	//	if (!UserId().empty()) {
	//		hstring path(UserId() + L"/picture");

	//		auto objectFactory = JsonClassFactory([](hstring jsonText) -> IInspectable
	//		{
	//			return Graph::FBProfilePictureData::FromJson(jsonText);
	//		});

	//		winsdkfb::FBSingleValue value = make<FBSingleValue>(
	//			path,
	//			parameters,
	//			objectFactory);
	//		result = co_await value.GetAsync();
	//	}

	//	co_return result;
	//}
}
