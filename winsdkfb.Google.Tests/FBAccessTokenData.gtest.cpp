#include "pch.h"

#include "FBAccessTokenData.h"

#include <winrt/Windows.Foundation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
TEST(FBAccessTokenDataTests, CreateInstance)
{
	// arrange
	auto instance = make_shared<winsdkfb::FBAccessTokenData>(L"SECRET_TOKEN", DateTime());

	// act

	// assert
	EXPECT_TRUE(instance != nullptr);
}

TEST(FBAccessTokenDataTests, CreateInstanceOfFBResult)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");
	shared_ptr<winsdkfb::FBResult> fbResult = winsdkfb::FBAccessTokenData::FromUri(uri);

	// act
	shared_ptr<winsdkfb::FBAccessTokenData> fbAccessTokenData = static_pointer_cast<winsdkfb::FBAccessTokenData>(fbResult);

	// assert
	EXPECT_TRUE(fbAccessTokenData != nullptr);
	EXPECT_TRUE(fbResult->Succeeded());
}

TEST(FBAccessTokenDataTests, CreateInstanceFromUri)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");

	// act
	auto instance = winsdkfb::FBAccessTokenData::FromUri(uri);

	// assert
	EXPECT_TRUE(instance != nullptr);
	EXPECT_STREQ(L"ACCESS_TOKEN", instance->AccessToken().c_str());
	EXPECT_FALSE(instance->IsExpired());
}

TEST(FBAccessTokenDataTests, IsExpiredYesterdayShouldBeTrue)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=-123456");

	// act
	auto instance = winsdkfb::FBAccessTokenData::FromUri(uri);

	// assert
	EXPECT_TRUE(instance != nullptr);
	EXPECT_TRUE(instance->IsExpired());
}
