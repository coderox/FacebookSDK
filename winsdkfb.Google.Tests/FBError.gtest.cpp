#include "pch.h"

#include "FBError.h"
#include <winrt/Windows.Foundation.h>

TEST(FBErrorTests, CreateInstance)
{
	// arrange
	std::wstring type(L"type");
	std::wstring message(L"message");

	// act
	auto fbError = winsdkfb::FBError(0, type, message);

	// assert
	EXPECT_EQ(type, fbError.Type());
	EXPECT_EQ(message, fbError.Message());
}

TEST(FBErrorTests, CreationFromJson)
{
	// arrange
	std::wstring json(L"{\"message\":\"message\", \"type\":\"type\"}");

	// act
	auto fbError = winsdkfb::FBError::FromJson(json);

	// assert
	EXPECT_EQ(L"type", fbError.Type());
	EXPECT_EQ(L"message", fbError.Message());
}

TEST(FBErrorTests, AnotherCreationFromJson)
{
	// arrange
	std::wstring json(L"{\"error\": {\"message\": \"Message describing the error\", \"type\": \"OAuthException\", \"code\": 190, \"error_subcode\": 460, \"error_user_title\": \"A title\", \"error_user_msg\": \"A message\"}}");

	// act
	auto fbError = winsdkfb::FBError::FromJson(json);

	// assert
	EXPECT_EQ(L"Message describing the error", fbError.Message());
	EXPECT_EQ(L"OAuthException", fbError.Type());
	EXPECT_EQ(190, fbError.Code());
	EXPECT_EQ(460, fbError.SubCode());
	EXPECT_EQ(L"A title", fbError.ErrorUserTitle());
	EXPECT_EQ(L"A message", fbError.ErrorUserMessage());
}

TEST(FBErrorTests, AppropriateFBErrorJsonMessage)
{
	// arrange
	std::wstring json(L"{ \
				\"message\": \"Error validating access token: User USER_ID has not authorized application APP_ID.\", \
				\"type\" : \"OAuthException\", \
				\"code\" : 190, \
				\"error_subcode\" : 458, \
				\"fbtrace_id\" : \"H2il2t5bn4e\"}");

	// act
	auto fbError = winsdkfb::FBError::FromJson(json);

	// assert
	EXPECT_EQ(L"OAuthException", fbError.Type());
	EXPECT_EQ(190, fbError.Code());
	EXPECT_EQ(458, fbError.SubCode());
}

TEST(FBErrorTests, CreationFromUri)
{
	// arrange
	winrt::Windows::Foundation::Uri uri(L"http://www.facebook.com/?error_reason=user_denied&error=access_denied&error_description=Permissions+error");

	// act
	auto fbError = winsdkfb::FBError::FromUri(uri);

	// assert
	EXPECT_EQ(L"Permissions error", fbError.Message());
	EXPECT_EQ(L"user_denied", fbError.Type());
}
