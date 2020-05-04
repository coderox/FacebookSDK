#include "pch.h"

#include "FBAccessTokenData.h"

#include <winrt/Windows.Foundation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
TEST(FBAccessTokenDataTests, CreateInstance)
{
	// arrange
	auto instance = winsdkfb::FBAccessTokenData(L"SECRET_TOKEN", DateTime());

	// act

	// assert
	EXPECT_FALSE(instance.AccessToken().empty());
}

TEST(FBAccessTokenDataTests, CreateInstanceOfFBResult)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");
	winsdkfb::FBResult fbResult = winsdkfb::FBAccessTokenData::FromUri(uri);

	// act
	winsdkfb::FBAccessTokenData& fbAccessTokenData = static_cast<winsdkfb::FBAccessTokenData&>(fbResult);

	// assert
	EXPECT_TRUE(fbAccessTokenData.Succeeded());
}

TEST(FBAccessTokenDataTests, CreateInstanceFromUri)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");

	// act
	auto instance = winsdkfb::FBAccessTokenData::FromUri(uri);

	// assert
	EXPECT_STREQ(L"ACCESS_TOKEN", instance.AccessToken().c_str());
	EXPECT_FALSE(instance.IsExpired());
}

TEST(FBAccessTokenDataTests, IsExpiredYesterdayShouldBeTrue)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=-123456");

	// act
	auto instance = winsdkfb::FBAccessTokenData::FromUri(uri);

	// assert
	EXPECT_TRUE(instance.IsExpired());
}
