#include "pch.h"

#include "FBAccessTokenData.h"

#include <winrt/Windows.Foundation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;

TEST(FBAccessTokenDataTests, Simple)
{
	// arrange
	auto instance = winsdkfb::FBAccessTokenData(L"SECRET_TOKEN", DateTime());

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
	auto instance = winsdkfb::FBAccessTokenData(L"SECRET_TOKEN", DateTime());

	// act
	auto accessToken = instance.AccessToken();

	// assert
	EXPECT_FALSE(accessToken.empty());
}

TEST(FBAccessTokenDataTests, CreateInstanceOfFBResult)
{
	// arrange
	Uri uri(L"https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");
	auto instance = winsdkfb::FBAccessTokenData::FromUri(uri);
	
	// act
	winsdkfb::FBResult fbResult(instance);
	auto fbAccessTokenData = fbResult.Object<winsdkfb::FBAccessTokenData>();

	// assert
	EXPECT_TRUE(fbResult.Succeeded());
	EXPECT_STREQ(fbAccessTokenData->AccessToken().c_str(), L"ACCESS_TOKEN");
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
