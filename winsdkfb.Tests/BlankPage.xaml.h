//
// BlankPage.xaml.h
// Declaration of the BlankPage class
//

#pragma once

#include "BlankPage.g.h"

namespace winsdkfb_Tests
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BlankPage sealed
	{
	public:
		BlankPage();
	
	private:
		void SetSessionAppIds();
		winsdkfb::FBPermissions^ BuildPermissions();
		void OnLoginClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnFeedClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnRequestsClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnSendClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnUserInfoFetched(winsdkfb::FBLoginButton^ sender, winsdkfb::Graph::FBUser^ userInfo);
		void OnReauthorizeClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
