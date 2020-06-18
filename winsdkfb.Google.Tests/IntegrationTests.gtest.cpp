#include "pch.h"

#include <winrt/Windows.Data.Json.h>
#include <pplawait.h>

#include "MockHttpClient.h"
#include "HttpManager.h"
#include "Utilities.h"
#include "FBSingleValue.h"
#include "Graph/FBUser.h"
#include "FBPaginatedArray.h"
#include <map>

TEST(IntegrationTests, MessagePolling) {
	// arrange
	auto mockHttpClient = std::make_shared<MockHttpClient>();
	winsdkfb::HttpManager::Instance()->SetHttpClient(mockHttpClient);
	// test no values returned from request
	mockHttpClient->ResponseData(L"{\"data\":[{\"from\":\"Micke\"},{\"from\":\"Magnus\"},{\"from\":\"Aron\"}]}");

	// Path
	auto graphPath = L"/12345/apprequests";

	// Properties
	std::unordered_map<winrt::hstring, winrt::hstring> properties;
	winrt::hstring data(L"data");
	properties[L"fields"] = data;

	// Conversion
	winsdkfb::JsonClassFactory fact = [](winrt::hstring JsonText) -> winsdkfb::FBResult {
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

	// act
	auto likes = winsdkfb::FBSingleValue(graphPath, properties, fact);
	auto result = concurrency::create_task(likes.GetAsync()).get();

	// assert
	EXPECT_TRUE(result.Succeeded());
	EXPECT_EQ(result.Object<int>(), 3);
}

TEST(IntegrationTests, FBPaginatedArray) {
	// arrange
	auto mockHttpClient = std::make_shared<MockHttpClient>();
	winsdkfb::HttpManager::Instance()->SetHttpClient(mockHttpClient);
	// test no values returned from request
	mockHttpClient->ResponseData(L"{\"data\":[{\"first_name\":\"Johan\",\"last_name\":\"Lindfors\",\"name\":\"Johan Lindfors\",\"id\":\"10156062009459646\",\"picture\":{\"data\":{\"height\":50,\"is_silhouette\":false,\"url\":\"https://platform-lookaside.fbsbx.com/platform/profilepic/?asid=10156062009459646&height=50&width=50&ext=1550996804&hash=AeTlVQ4Q_fIUqP_n\",\"width\":50}}}]}");

	auto graphPath = L"/12345/users";

	winsdkfb::JsonClassFactory fact = [](winrt::hstring JsonText) -> winsdkfb::FBResult
	{
		return winsdkfb::FBResult(winsdkfb::Graph::FBUser::FromJson(JsonText));
	};

	// act
	auto likes = winsdkfb::FBPaginatedArray(graphPath, {}, fact);
	auto result = concurrency::create_task(likes.FirstAsync()).get();
	auto users = result.Object<std::vector<winsdkfb::FBResult>>();
	auto user = users->at(0).Object<winsdkfb::Graph::FBUser>();

	// assert
	EXPECT_TRUE(result.Succeeded());
	EXPECT_EQ(users->size(), 1);
	EXPECT_STREQ(user->FirstName().c_str(), L"Johan");
}
