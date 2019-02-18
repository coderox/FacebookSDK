#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(GraphUriBuilder_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto instance = ref new winsdkfb::GraphUriBuilder(L"http://www.coderox.se/");

			// act

			// assert
			Assert::IsNotNull(instance);
		}
	};
}
