#include "pch.h"

#include "MainPage.h"
#include "MainPage.g.cpp"
#include "MockHttpClient.h"
#include "HttpManager.h"
#include "Utilities.h"
#include "FBPaginatedArray.h"
#include "Graph/FBUser.h"
#include "FBResult.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

namespace winrt::FacebookSDK_TestClient::implementation
{
	MainPage::MainPage()
	{
		InitializeComponent();
	}

	const wchar_t* requested_permissions[] = {
		L"public_profile",
		L"user_friends",
		L"user_likes",
		L"user_location",
		L"publish_pages",
		L"manage_pages",
		L"user_posts"
	};

	void SetSessionAppIds() {
		//auto session = winsdkfb::FBSession::ActiveSession();
		//session->FBAppId("719494811465102");

		auto fbResult = std::make_shared<winsdkfb::FBResult>();
	}

	//winsdkfb::FBPermissions^ BlankPage::BuildPermissions() {
	//	auto v = ref new Vector<String^>();
	//	for (auto const& permission : requested_permissions)
	//	{
	//		v->Append(ref new String(permission));
	//	}
	//	return ref new winsdkfb::FBPermissions(v->GetView());
	//}

	//void BlankPage::OnLoginClicked(Object^ sender, RoutedEventArgs^ e)
	//{
	//	auto session = winsdkfb::FBSession::ActiveSession;
	//	if (session->LoggedIn) {
	//		create_task(session->LogoutAsync());
	//		loginButton->Content = ref new Platform::String(L"Login");
	//		profilePicture->UserId = "";
	//	}
	//	else {
	//		SetSessionAppIds();
	//		create_task(session->LoginAsync(BuildPermissions(), winsdkfb::SessionLoginBehavior::WebView)).then([&](winsdkfb::FBResult^ result) {
	//			auto session = winsdkfb::FBSession::ActiveSession;
	//			if (session->LoggedIn) {
	//				loginButton->Content = ref new Platform::String(L"Logout");
	//				OutputDebugString(session->AccessTokenData->AccessToken->Data());
	//				auto user = session->User;
	//				Platform::String^ userId = user->Id;
	//				profilePicture->UserId = userId;
	//			}
	//			else {
	//				if (!result->Succeeded && result->ErrorInfo != nullptr) {
	//					auto message = result->ErrorInfo->Message;
	//					OutputDebugString(message->Data());
	//				}
	//			}
	//			});
	//	}
	//}

	//void BlankPage::OnFeedClicked(Object^ sender, RoutedEventArgs^ e)
	//{
	//	auto session = winsdkfb::FBSession::ActiveSession;

	//	if (session->LoggedIn) {
	//		PropertySet^ params = ref new PropertySet();
	//		params->Insert(L"caption", L"I love Brussels Sprouts!");
	//		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");
	//		params->Insert(L"description", L"Om Nom Nom!");

	//		create_task(session->ShowFeedDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
	//			OutputDebugString(L"Showed 'Feed' dialog.\n");
	//			});
	//	}
	//}

	//void BlankPage::OnRequestsClicked(Object^ sender, RoutedEventArgs^ e)
	//{
	//	auto session = winsdkfb::FBSession::ActiveSession;

	//	if (session->LoggedIn) {
	//		PropertySet^ params = ref new PropertySet();
	//		params->Insert(L"title", L"I love Brussels Sprouts!");
	//		params->Insert(L"message", L"Om Nom Nom!");

	//		create_task(session->ShowRequestsDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
	//			OutputDebugString(L"Showed 'Requests' dialog.\n");
	//			});
	//	}
	//}

	//void BlankPage::OnSendClicked(Object^ sender, RoutedEventArgs^ e)
	//{
	//	auto session = winsdkfb::FBSession::ActiveSession;

	//	if (session->LoggedIn) {
	//		PropertySet^ params = ref new PropertySet();
	//		params->Insert(L"link", L"https://en.wikipedia.org/wiki/Brussels_sprout");

	//		create_task(session->ShowSendDialogAsync(params)).then([=](winsdkfb::FBResult^ dialogResult) {
	//			OutputDebugString(L"Showed 'Send' dialog.\n");
	//			});
	//	}
	//}

	//void winsdkfb_Tests::BlankPage::OnUserInfoFetched(winsdkfb::FBLoginButton^ sender, winsdkfb::Graph::FBUser^ userInfo)
	//{
	//	profilePicture->UserId = userInfo->Id;
	//}


	//void winsdkfb_Tests::BlankPage::OnPostClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
	//{
	//	winsdkfb::FBSession^ sess = winsdkfb::FBSession::ActiveSession;
	//	if (sess->LoggedIn)
	//	{
	//		// Set caption, link and description parameters
	//		PropertySet^ parameters = ref new PropertySet();
	//		parameters->Insert(L"caption", L"Microsoft");
	//		parameters->Insert(L"link", L"https://www.microsoft.com/en-us/default.aspx");
	//		parameters->Insert(L"description", L"Microsoft home page");

	//		// Add message
	//		parameters->Insert(L"message", L"Posting from my Universal Windows app.");

	//		//Create Graph API path
	//		String^ graphPath = sess->User->Id + L"/feed";

	//		// Create a json class factory with a class (FBReturnObject class)
	//		// that can receive and parse the json response returned
	//		winsdkfb::JsonClassFactory^ fact = ref new winsdkfb::JsonClassFactory([](String^ JsonText) ->
	//			Object^
	//			{
	//				auto returnObject = ref new FBReturnObject();
	//				returnObject->Id = Windows::Data::Json::JsonObject::Parse(JsonText)->GetNamedString("id");
	//				return returnObject;
	//			});

	//		winsdkfb::FBSingleValue^ sval = ref new winsdkfb::FBSingleValue(graphPath, parameters, fact);
	//		create_task(sval->PostAsync()).then([=](winsdkfb::FBResult^ result)
	//			{
	//				if (result->Succeeded)
	//				{
	//					FBReturnObject^ response = static_cast<FBReturnObject^>(result->Object);
	//				}
	//				else
	//				{
	//					// Posting failed
	//				}
	//			});

	//	}
	//}


	//void winsdkfb_Tests::BlankPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
	//{
	//	SetSessionAppIds();

	//	auto session = winsdkfb::FBSession::ActiveSession;
	//	if (!session->LoggedIn) {
	//		create_task(session->LoginAsync(BuildPermissions(), winsdkfb::SessionLoginBehavior::Silent)).then([&](winsdkfb::FBResult^ result) {
	//			auto session = winsdkfb::FBSession::ActiveSession;
	//			if (session->LoggedIn) {
	//				loginButton->Content = ref new Platform::String(L"Logout");
	//				OutputDebugString(session->AccessTokenData->AccessToken->Data());
	//				auto user = session->User;
	//				Platform::String^ userId = user->Id;
	//				profilePicture->UserId = userId;
	//			}
	//			else {
	//				if (!result->Succeeded && result->ErrorInfo != nullptr) {
	//					auto message = result->ErrorInfo->Message;
	//					OutputDebugString(message->Data());
	//				}
	//			}
	//			});
	//	}
	//}

	void MainPage::OnLoaded(IInspectable const&, RoutedEventArgs const&)
	{
		SetSessionAppIds();
	}

	void MainPage::OnLoginClicked(IInspectable const&, RoutedEventArgs const&)
	{

	}

	void MainPage::OnFeedClicked(IInspectable const&, RoutedEventArgs const&)
	{

	}

	void MainPage::OnRequestsClicked(IInspectable const&, RoutedEventArgs const&)
	{
		auto mockHttpClient = std::make_shared<MockHttpClient>();
		winsdkfb::HttpManager::Instance()->SetHttpClient(mockHttpClient);
		// test no values returned from request
		mockHttpClient->ResponseData(L"{\"data\":[{\"first_name\":\"Johan\",\"last_name\":\"Lindfors\",\"name\":\"Johan Lindfors\",\"id\":\"10156062009459646\",\"picture\":{\"data\":{\"height\":50,\"is_silhouette\":false,\"url\":\"https://platform-lookaside.fbsbx.com/platform/profilepic/?asid=10156062009459646&height=50&width=50&ext=1550996804&hash=AeTlVQ4Q_fIUqP_n\",\"width\":50}}}]}");

		auto graphPath = L"/12345/users";

		winsdkfb::JsonClassFactory fact = [](winrt::hstring JsonText) -> winsdkfb::Graph::FBUser
		{
			return winsdkfb::Graph::FBUser::FromJson(JsonText);
		};

		auto likes = winsdkfb::FBPaginatedArray(graphPath, nullptr, fact);
		auto result = concurrency::create_task(likes.FirstAsync()).get();

		bool succeeded = result.Succeeded();
		//auto users = dynamic_cast<IVectorView<Object^>^>(result->Object);
		//auto user = dynamic_cast<winsdkfb::Graph::FBUser^>(users->GetAt(0));
		//auto firstName = user.FirstName;
	}


	void MainPage::OnSendClicked(IInspectable const&, RoutedEventArgs const&)
	{

	}

	void MainPage::OnPostClicked(IInspectable const&, RoutedEventArgs const&)
	{

	}
}