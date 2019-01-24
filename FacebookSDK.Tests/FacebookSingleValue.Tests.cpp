#include "pch.h"
#include "CppUnitTest.h"
#include "../FacebookSDK/FacebookSingleValue.h"
#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
	TEST_CLASS(FacebookSingleValue_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto request = ref new Platform::String(L"");
			// act
			auto instance = ref new FacebookSDK::FacebookSingleValue(request, nullptr, nullptr);

			// assert
			Assert::IsNotNull(instance);
		}
	};
}
