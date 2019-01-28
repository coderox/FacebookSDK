//
// BlankPage.xaml.cpp
// Implementation of the BlankPage class
//

#include "pch.h"
#include "BlankPage.xaml.h"
#include "MockHttpClient.h"
#include <pplawait.h>

using namespace FacebookSDK_Tests;
using namespace concurrency;

using namespace Platform;
using namespace Platform::Collections;
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

const wchar_t* requested_permissions[] = {
	L"public_profile",
	L"user_friends",
	L"user_likes",
	L"user_location"
};

BlankPage::BlankPage()
{
	InitializeComponent();

	//MockHttpClient^ mockHttpClient = ref new MockHttpClient();
	//FacebookSDK::HttpManager::Instance->SetHttpClient(mockHttpClient);
	//// test no values returned from request
	//mockHttpClient->ResponseData = L"{\"data\":[]}";
	//String^ graphPath = L"/12345 / likes";

	//auto fact = ref new FacebookSDK::JsonClassFactory([=](Platform::String^ JsonText)
	//{ 
	//	return JsonText; 
	//});

	//auto likes = ref new FacebookSDK::FacebookPaginatedArray(graphPath, nullptr, fact);
	//auto result = concurrency::create_task(likes->FirstAsync()).get();
	SetSessionAppIds();
}

void BlankPage::SetSessionAppIds() {
	auto session = FacebookSDK::FacebookSession::ActiveSession;
	session->FacebookAppId = "719494811465102";
}

FacebookSDK::FacebookPermissions^ BlankPage::BuildPermissions() {
	auto v = ref new Vector<String^>();
	for (auto const& permission : requested_permissions)
	{
		v->Append(ref new String(permission));
	}
	return ref new FacebookSDK::FacebookPermissions(v->GetView());
}


void BlankPage::OnLoginClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = FacebookSDK::FacebookSession::ActiveSession;

	if (session->LoggedIn) {
		create_task(session->LogoutAsync());
		loginButton->Content = ref new Platform::String(L"Login");
	}
	else {
		create_task(session->LoginAsync(BuildPermissions(), FacebookSDK::SessionLoginBehavior::WebView)).then([&](FacebookSDK::FacebookResult^ result) {
			auto session = FacebookSDK::FacebookSession::ActiveSession;
			if (session->LoggedIn) {
				loginButton->Content = ref new Platform::String(L"Logout");
				OutputDebugString(session->AccessTokenData->AccessToken->Data());
				auto user = session->User;
				Platform::String^ userId = user->Id;
				profilePicture->UserId = userId;
			}
			else {
				if (!result->Succeeded && result->ErrorInfo != nullptr) {
					auto message = result->ErrorInfo->Message;
					OutputDebugString(message->Data());
				}
			}
		});
	}
}

void BlankPage::OnFeedClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = FacebookSDK::FacebookSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"caption", L"I love Brussels Sprouts!");
		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");
		params->Insert(L"description", L"Om Nom Nom!");

		create_task(session->ShowFeedDialogAsync(params)).then([=](FacebookSDK::FacebookResult^ dialogResult) {
			OutputDebugString(L"Showed 'Feed' dialog.\n");
		});
	}
}

void BlankPage::OnRequestsClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = FacebookSDK::FacebookSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"title", L"I love Brussels Sprouts!");
		params->Insert(L"message", L"Om Nom Nom!");

		create_task(session->ShowRequestsDialogAsync(params)).then([=](FacebookSDK::FacebookResult^ dialogResult) {
			OutputDebugString(L"Showed 'Requests' dialog.\n");
		});
	}
}

void BlankPage::OnSendClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = FacebookSDK::FacebookSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");
		
		create_task(session->ShowSendDialogAsync(params)).then([=](FacebookSDK::FacebookResult^ dialogResult) {
			OutputDebugString(L"Showed 'Send' dialog.\n");
		});
	}
}

void FacebookSDK_Tests::BlankPage::OnUserInfoFetched(FacebookSDK::FacebookLoginButton^ sender, FacebookSDK::Graph::FBUser^ userInfo)
{
	profilePicture->UserId = userInfo->Id;
}
