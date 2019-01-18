#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation;

namespace FacebookSDK_Tests
{
	TEST_CLASS(FacebookAccessTokenData_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto instance = ref new FacebookSDK::FacebookAccessTokenData(L"SECRET_TOKEN", Windows::Foundation::DateTime());

			// act

			// assert
			Assert::IsNotNull(instance);
		}

		TEST_METHOD(TestCreateInstanceFromUri)
		{
			// arrange
			auto uri = ref new Uri("https://www.facebook.com/connect/login_success.html#access_token=ACCESS_TOKEN&expires_in=123456");

			// act
			auto instance = FacebookSDK::FacebookAccessTokenData::FromUri(uri);

			// assert
			Assert::IsNotNull(instance);
			Assert::AreEqual("ACCESS_TOKEN", instance->AccessToken);
		}

	};
}
