#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
	};
}
