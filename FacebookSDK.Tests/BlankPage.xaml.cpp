//
// BlankPage.xaml.cpp
// Implementation of the BlankPage class
//

#include "pch.h"
#include "BlankPage.xaml.h"
#include "MockHttpClient.h"

using namespace FacebookSDK_Tests;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

BlankPage::BlankPage()
{
	InitializeComponent();

	MockHttpClient^ mockHttpClient = ref new MockHttpClient();
	FacebookSDK::HttpManager::Instance->SetHttpClient(mockHttpClient);
	// test no values returned from request
	mockHttpClient->ResponseData = L"{\"data\":[]}";
	String^ graphPath = L"/12345 / likes";

	auto fact = ref new FacebookSDK::JsonClassFactory([=](Platform::String^ JsonText)
	{ 
		return JsonText; 
	});

	auto likes = ref new FacebookSDK::FacebookPaginatedArray(graphPath, nullptr, fact);
	auto result = concurrency::create_task(likes->FirstAsync()).get();
}
