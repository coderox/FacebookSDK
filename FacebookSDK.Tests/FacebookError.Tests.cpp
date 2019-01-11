#include "pch.h"
#include "CppUnitTest.h"
#include "../FacebookSDK/FacebookError.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
    TEST_CLASS(FacebookError_Tests)
    {
    public:
        TEST_METHOD(TestCreateInstance)
        {
			// arrange
			auto type = ref new Platform::String(L"type");
			auto message = ref new Platform::String(L"message");

			// act
			auto fbError = ref new FacebookSDK::FacebookError(0, type, message);

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
			auto fbError = FacebookSDK::FacebookError::FromJson(json);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(fbError->Message, L"message");
			Assert::AreEqual(fbError->Type, L"type");
		}

		TEST_METHOD(TestAppropriateFacebookErrorJsonMessage)
		{
			// arrange
			auto json = ref new Platform::String(L"{ \
				\"message\": \"Error validating access token: User USER_ID has not authorized application APP_ID.\", \
				\"type\" : \"OAuthException\", \
				\"code\" : 190, \
				\"error_subcode\" : 458, \
				\"fbtrace_id\" : \"H2il2t5bn4e\"}");
			
			// act
			auto fbError = FacebookSDK::FacebookError::FromJson(json);

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
			auto fbError = FacebookSDK::FacebookError::FromUri(uri);

			// assert
			Assert::IsNotNull(fbError);
			Assert::AreEqual(L"Permissions error", fbError->Message);
			Assert::AreEqual(L"user_denied", fbError->Type);
		}
    };
}