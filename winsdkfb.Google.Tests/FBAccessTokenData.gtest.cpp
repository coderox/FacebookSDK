#include "pch.h"

#include "FBAccessTokenData.h"

#include <winrt/Windows.Foundation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;

TEST(FBAccessTokenDataTests, Simple)
{
	// arrange
	auto instance = winsdkfb::FBAccessTokenData(L"SECRET_TOKEN", DateTime(), DateTime());

	// act
	auto fbResult = winsdkfb::FBResult(instance);
	auto fbAccessTokenData = fbResult.Object<winsdkfb::FBAccessTokenData>();

	// assert
	EXPECT_STREQ(instance.AccessToken().c_str(), L"SECRET_TOKEN");
	EXPECT_TRUE(fbResult.Succeeded());
	EXPECT_STREQ(fbAccessTokenData->AccessToken().c_str(), L"SECRET_TOKEN");
}

TEST(FBAccessTokenDataTests, CreateInstance)
{
	// arrange
	auto instance = winsdkfb::FBAccessTokenData(L"SECRET_TOKEN", DateTime(), DateTime());

	// act
	auto accessToken = instance.AccessToken();

	// assert
	EXPECT_FALSE(accessToken.empty());
}

//TEST(FBAccessTokenDataTests, CreateInstanceOfFBResult)
//{
//	// arrange
//	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456&data_access_expiration_time=123456");
//	
//	// act
//	auto instance = std::any_cast<winsdkfb::FBAccessTokenData>(winsdkfb::FBAccessTokenData::FromUri(uri));
//	winsdkfb::FBResult fbResult(instance);
//	auto fbAccessTokenData = fbResult.Object<winsdkfb::FBAccessTokenData>();
//
//	// assert
//	EXPECT_TRUE(fbResult.Succeeded());
//	EXPECT_STREQ(fbAccessTokenData->AccessToken().c_str(), L"ACCESS_TOKEN");
//}
//
//TEST(FBAccessTokenDataTests, CreateInstanceFromUri)
//{
//	// arrange
//	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456&data_access_expiration_time=123456");
//
//	// act
//	auto instance = std::any_cast<winsdkfb::FBAccessTokenData>(winsdkfb::FBAccessTokenData::FromUri(uri));
//
//	// assert
//	EXPECT_STREQ(L"ACCESS_TOKEN", instance.AccessToken().c_str());
//	EXPECT_FALSE(instance.IsExpired());
//}

//TEST(FBAccessTokenDataTests, IsExpiredYesterdayShouldBeTrue)
//{
//	// arrange
//	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=-123456&data_access_expiration_time=123456");
//
//	// act
//	auto instance = std::any_cast<winsdkfb::FBAccessTokenData>(winsdkfb::FBAccessTokenData::FromUri(uri));
//
//	// assert
//	EXPECT_TRUE(instance.IsExpired());
//}
//
//TEST(FBAccessTokenDataTests, UsingAPreviousProperUrl)
//{
//	// arrange
//	Uri uri(L"https://apps.facebook.com/ff_social_sample_dev/connect/login_success.html?#access_token=EAAGpgur8Un0BAO0ZCehZAL1GHFjmBdVOtZBJuZCcln3fecH4ZB4QS8TwGtapmtaLCFWZBXxPyoygHTn3YPrdWJq6o52MPdb6ykZBEzr9vQ9cjEtaM1AiMNZC0FKkVWznRUYgF7eWaPRZAagVCz7ZAi1piPhZAYtiYepNNi9TdsmbKvAv9f3BgFxaokgWfLCPnxYENhASV5ELj751o1NXQVcHZC0j&data_access_expiration_time=1597150167&expires_in=4233");
//
//	// act
//	auto instance = std::any_cast<winsdkfb::FBAccessTokenData>(winsdkfb::FBAccessTokenData::FromUri(uri));
//
//	// assert
//	EXPECT_FALSE(instance.IsExpired());
//	EXPECT_FALSE(instance.IsDataAccessExpired());
//}
