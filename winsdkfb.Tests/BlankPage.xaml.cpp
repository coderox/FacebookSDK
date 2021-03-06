﻿//
// BlankPage.xaml.cpp
// Implementation of the BlankPage class
//

#include "pch.h"
#include "BlankPage.xaml.h"
#include "MockHttpClient.h"
#include "FBReturnObject.h"
#include <pplawait.h>

using namespace winsdkfb_Tests;
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
	L"user_location",
	L"publish_pages",
	L"manage_pages",
	L"user_posts"
};

BlankPage::BlankPage()
{
	InitializeComponent();

	//MockHttpClient^ mockHttpClient = ref new MockHttpClient();
	//winsdkfb::HttpManager::Instance->SetHttpClient(mockHttpClient);
	//// test no values returned from request
	//mockHttpClient->ResponseData = L"{\"data\":[]}";
	//String^ graphPath = L"/12345 / likes";

	//auto fact = ref new winsdkfb::JsonClassFactory([=](Platform::String^ JsonText)
	//{ 
	//	return JsonText; 
	//});

	//auto likes = ref new winsdkfb::FBPaginatedArray(graphPath, nullptr, fact);
	//auto result = concurrency::create_task(likes->FirstAsync()).get();
}

void BlankPage::SetSessionAppIds() {
	auto session = winsdkfb::FBSession::ActiveSession;
	session->FBAppId = "719494811465102";
}

winsdkfb::FBPermissions^ BlankPage::BuildPermissions() {
	auto v = ref new Vector<String^>();
	for (auto const& permission : requested_permissions)
	{
		v->Append(ref new String(permission));
	}
	return ref new winsdkfb::FBPermissions(v->GetView());
}


void BlankPage::OnLoginClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = winsdkfb::FBSession::ActiveSession;
	if (session->LoggedIn) {
		create_task(session->LogoutAsync());
		loginButton->Content = ref new Platform::String(L"Login");
		profilePicture->UserId = "";
	}
	else {
		SetSessionAppIds();
		create_task(session->LoginAsync(BuildPermissions(), winsdkfb::SessionLoginBehavior::WebView)).then([&](winsdkfb::FBResult^ result) {
			auto session = winsdkfb::FBSession::ActiveSession;
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
	auto session = winsdkfb::FBSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"caption", L"I love Brussels Sprouts!");
		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");
		params->Insert(L"description", L"Om Nom Nom!");

		create_task(session->ShowFeedDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
			OutputDebugString(L"Showed 'Feed' dialog.\n");
		});
	}
}

void BlankPage::OnRequestsClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = winsdkfb::FBSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"title", L"I love Brussels Sprouts!");
		params->Insert(L"message", L"Om Nom Nom!");

		create_task(session->ShowRequestsDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
			OutputDebugString(L"Showed 'Requests' dialog.\n");
		});
	}
}

void BlankPage::OnSendClicked(Object^ sender, RoutedEventArgs^ e)
{
	auto session = winsdkfb::FBSession::ActiveSession;

	if (session->LoggedIn) {
		PropertySet^ params = ref new PropertySet();
		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");
		
		create_task(session->ShowSendDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
			OutputDebugString(L"Showed 'Send' dialog.\n");
		});
	}
}

void winsdkfb_Tests::BlankPage::OnUserInfoFetched(winsdkfb::FBLoginButton^ sender, winsdkfb::Graph::FBUser^ userInfo)
{
	profilePicture->UserId = userInfo->Id;
}


void winsdkfb_Tests::BlankPage::OnPostClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	winsdkfb::FBSession^ sess = winsdkfb::FBSession::ActiveSession;
	if (sess->LoggedIn)
	{
		// Set caption, link and description parameters
		PropertySet^ parameters = ref new PropertySet();
		parameters->Insert(L"caption", L"Microsoft");
		parameters->Insert(L"link", L"https://www.microsoft.com/en-us/default.aspx");
		parameters->Insert(L"description", L"Microsoft home page");

		// Add message
		parameters->Insert(L"message", L"Posting from my Universal Windows app.");

		//Create Graph API path
		String^ graphPath = sess->User->Id + L"/feed";

		// Create a json class factory with a class (FBReturnObject class)
		// that can receive and parse the json response returned
		winsdkfb::JsonClassFactory^ fact = ref new winsdkfb::JsonClassFactory([](String^ JsonText) ->
			Object^
		{
			auto returnObject = ref new FBReturnObject();
			returnObject->Id = Windows::Data::Json::JsonObject::Parse(JsonText)->GetNamedString("id");
			return returnObject;
		});

		winsdkfb::FBSingleValue^ sval = ref new winsdkfb::FBSingleValue(graphPath, parameters, fact);
		create_task(sval->PostAsync()).then([=](winsdkfb::FBResult^ result)
		{
			if (result->Succeeded)
			{
				FBReturnObject^ response = static_cast<FBReturnObject ^>(result->Object);
			}
			else
			{
				// Posting failed
			}
		});

	}
}


void winsdkfb_Tests::BlankPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SetSessionAppIds(); 
	
	auto session = winsdkfb::FBSession::ActiveSession;
	if (!session->LoggedIn) {
		create_task(session->LoginAsync(BuildPermissions(), winsdkfb::SessionLoginBehavior::Silent)).then([&](winsdkfb::FBResult^ result) {
			auto session = winsdkfb::FBSession::ActiveSession;
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
