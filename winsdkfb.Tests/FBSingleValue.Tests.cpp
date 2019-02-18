#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FBSingleValue.h"
#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(FBSingleValue_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto request = ref new Platform::String(L"");
			// act
			auto instance = ref new winsdkfb::FBSingleValue(request, nullptr, nullptr);

			// assert
			Assert::IsNotNull(instance);
		}
	};
}
