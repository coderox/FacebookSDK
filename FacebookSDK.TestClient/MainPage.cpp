#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "MockHttpClient.h"
#include "HttpManager.h"
#include "Utilities.h"
#include "FBPaginatedArray.h"
#include "FBSingleValue.h"
#include "Graph/FBUser.h"
#include "FBResult.h"
#include "FBSession.h"
#include <map>

using namespace std;
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
		L"public_profile"
	};

	void SetSessionAppIds() {
		auto session = winsdkfb::FBSession::ActiveSession();
		session->FBAppId(L"719494811465102");
	}

	winsdkfb::FBPermissions BuildPermissions() {
		std::vector<wstring>v;
		for (auto const& permission : requested_permissions)
		{
			v.push_back(wstring(permission));
		}
		return winsdkfb::FBPermissions(v);
	}

	winrt::fire_and_forget LoginInternal() {
		auto session = winsdkfb::FBSession::ActiveSession();
		if (session->LoggedIn()) {
			co_await session->LogoutAsync();
		}
		else {
			SetSessionAppIds();
			auto result = co_await session->LoginAsync(BuildPermissions(), winsdkfb::SessionLoginBehavior::WebView);
			session = winsdkfb::FBSession::ActiveSession();
			if (session->LoggedIn()) {
				auto user = session->User();
				OutputDebugString(user.FirstName().c_str());
			}
			else {
				if (!result.Succeeded() && result.ErrorInfo().Code() > 0) {
					OutputDebugString(result.ErrorInfo().Message().c_str());
				}
			}
		}
	}

	void MainPage::OnLoaded(winrt::Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
	{
		SetSessionAppIds();

		LoginInternal();
	}

	void MainPage::OnLoginClicked(IInspectable const&, RoutedEventArgs const&)
	{
		LoginInternal();
	}

	winrt::fire_and_forget OnFeedClickedInternal()
	{
		auto session = winsdkfb::FBSession::ActiveSession();

		if (session->LoggedIn()) {
			std::unordered_map<wstring, wstring> params;
			params[L"caption"] = L"I love Brussels Sprouts!";
			params[L"link"] = L"https://en.wikipedia.org/wiki/Brussels_sprout";
			params[L"description"] =  L"Om Nom Nom!";

			auto dialogResult = co_await session->ShowFeedDialogAsync(params);
			OutputDebugString(L"Showed 'Feed' dialog.\n");
		}
	}
	
	void MainPage::OnFeedClicked(IInspectable const&, RoutedEventArgs const&)
	{
		OnFeedClickedInternal();
	}

	winrt::fire_and_forget OnRequestsClickedInternal() {
		auto session = winsdkfb::FBSession::ActiveSession();

		if (session->LoggedIn()) {
			std::unordered_map<wstring, wstring> params;
			params[L"title"] = L"I love Brussels Sprouts!";
			params[L"message"] = L"Om Nom Nom!";

			auto dialogResult = co_await session->ShowRequestsDialogAsync(params);
			OutputDebugString(L"Showed 'Requests' dialog");
		}
	}

	void MainPage::OnRequestsClicked(IInspectable const&, RoutedEventArgs const&)
	{
		OnRequestsClickedInternal();
	}

	winrt::fire_and_forget OnSendClickedInternalWithMock()
	{
		auto mockHttpClient = std::make_shared<MockHttpClient>();
		winsdkfb::HttpManager::Instance()->SetHttpClient(mockHttpClient);

		// test values returned from request
		mockHttpClient->ResponseData(L"{\"data\":[{\"from\":\"Micke\"},{\"from\":\"Magnus\"},{\"from\":\"Aron\"}]}");

		// test no values returned from request
		//mockHttpClient->ResponseData(L"{\"data\":[]}");

		auto graphPath = L"/12345/apprequests";

		winsdkfb::JsonClassFactory fact = [](wstring JsonText) -> winsdkfb::FBResult {
			int numberOfMessages = 0;
			try {
				auto messages = winrt::Windows::Data::Json::JsonObject::Parse(JsonText).GetNamedArray(L"data");
				numberOfMessages = messages.Size();
			}
			catch (...) {
				numberOfMessages = 0;
			}
			return winsdkfb::FBResult(numberOfMessages);
		};

		std::unordered_map<wstring, wstring> properties;
		properties[L"fields"] = L"data";

		auto likes = winsdkfb::FBSingleValue(graphPath, properties, fact);
		auto result = co_await likes.GetAsync();

		if (result.Succeeded()) {
			auto resultObject = result.Object<int>();
			if (resultObject > 0) {
				OutputDebugString(L"Received a number of responses");
			} else {
				OutputDebugString(L"Received zero responses");
			}
		}
	}

	winrt::fire_and_forget OnSendClickedInternal()
	{
		auto session = winsdkfb::FBSession::ActiveSession();

		if (session->LoggedIn()) {
			std::unordered_map<wstring, wstring> params;
			params[L"link"] = L"https://en.wikipedia.org/wiki/Brussels_sprout";

			auto dialogResult = co_await create_task(session->ShowSendDialogAsync(params));
			OutputDebugString(L"Showed 'Send' dialog.\n");
		}
	}

	void MainPage::OnSendClicked(IInspectable const&, RoutedEventArgs const&)
	{
		OnSendClickedInternal();
		//OnSendClickedInternalWithMock();
	}
}