#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FBResult.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(FBResult_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto fbResult = ref new winsdkfb::FBResult(nullptr);

			// act

			// assert
			Assert::IsNotNull(fbResult);
		}

		TEST_METHOD(TestCreateInstanceWithNullptrShouldNotSucceed)
		{
			// arrange
			auto fbResult = ref new winsdkfb::FBResult(nullptr);

			// act
		
			// assert
			Assert::IsNotNull(fbResult);
			Assert::IsFalse(fbResult->Succeeded);
		}

		TEST_METHOD(TestCreateInstanceWithErrorShouldHaveError)
		{
			// arrange
			auto fbError = ref new winsdkfb::FBError(0,L"type",L"message");

			// act
			auto fbResult = ref new winsdkfb::FBResult(fbError);

			// assert
			Assert::IsNotNull(fbResult);
			Assert::IsNotNull(fbResult->ErrorInfo);
			Assert::AreEqual(L"type", fbResult->ErrorInfo->Type);
		}
	};
}
