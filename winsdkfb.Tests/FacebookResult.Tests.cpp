#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FacebookResult.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(FacebookResult_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto fbResult = ref new winsdkfb::FacebookResult(nullptr);

			// act

			// assert
			Assert::IsNotNull(fbResult);
		}

		TEST_METHOD(TestCreateInstanceWithNullptrShouldNotSucceed)
		{
			// arrange
			auto fbResult = ref new winsdkfb::FacebookResult(nullptr);

			// act
		
			// assert
			Assert::IsNotNull(fbResult);
			Assert::IsFalse(fbResult->Succeeded);
		}

		TEST_METHOD(TestCreateInstanceWithErrorShouldHaveError)
		{
			// arrange
			auto fbError = ref new winsdkfb::FacebookError(0,L"type",L"message");

			// act
			auto fbResult = ref new winsdkfb::FacebookResult(fbError);

			// assert
			Assert::IsNotNull(fbResult);
			Assert::IsNotNull(fbResult->ErrorInfo);
			Assert::AreEqual(L"type", fbResult->ErrorInfo->Type);
		}
	};
}
