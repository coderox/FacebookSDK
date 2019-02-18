#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FBError.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
    TEST_CLASS(FBError_Tests)
    {
    public:
        TEST_METHOD(TestCreateInstance)
        {
			// arrange
			auto type = ref new Platform::String(L"type");
			auto message = ref new Platform::String(L"message");

			// act
			auto fbError = ref new winsdkfb::FBError(0, type, message);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(type, fbError->Type);
			Assert::AreEqual(message, fbError->Message);
        }

		TEST_METHOD(TestCreationFromJson)
		{
			// arrange
			auto json = ref new Platform::String(L"{\"message\":\"message\", \"type\":\"type\"}");

			// act
			auto fbError = winsdkfb::FBError::FromJson(json);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(L"message", fbError->Message);
			Assert::AreEqual(L"type", fbError->Type);
		}

		TEST_METHOD(TestAnotherCreationFromJson)
		{
			// arrange
			auto json = ref new Platform::String(L"{\"error\": {\"message\": \"Message describing the error\", \"type\": \"OAuthException\", \"code\": 190, \"error_subcode\": 460, \"error_user_title\": \"A title\", \"error_user_msg\": \"A message\"}}");

			// act
			auto fbError = winsdkfb::FBError::FromJson(json);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(L"Message describing the error", fbError->Message);
			Assert::AreEqual(L"OAuthException", fbError->Type);
			Assert::AreEqual(190, fbError->Code);
			Assert::AreEqual(460, fbError->Subcode);
			Assert::AreEqual(L"A title", fbError->ErrorUserTitle);
			Assert::AreEqual(L"A message", fbError->ErrorUserMessage);
		}

		TEST_METHOD(TestAppropriateFBErrorJsonMessage)
		{
			// arrange
			auto json = ref new Platform::String(L"{ \
				\"message\": \"Error validating access token: User USER_ID has not authorized application APP_ID.\", \
				\"type\" : \"OAuthException\", \
				\"code\" : 190, \
				\"error_subcode\" : 458, \
				\"fbtrace_id\" : \"H2il2t5bn4e\"}");
			
			// act
			auto fbError = winsdkfb::FBError::FromJson(json);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(190, fbError->Code);
			Assert::AreEqual(458, fbError->Subcode);
			Assert::AreEqual("OAuthException", fbError->Type);
		}

		TEST_METHOD(TestCreationFromUri)
		{
			// arrange
			auto uri = ref new Windows::Foundation::Uri(L"http://www.facebook.com/?error_reason=user_denied&error=access_denied&error_description=Permissions+error");

			// act
			auto fbError = winsdkfb::FBError::FromUri(uri);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(L"Permissions error", fbError->Message);
			Assert::AreEqual(L"user_denied", fbError->Type);
		}
    };
}