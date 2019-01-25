#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
	TEST_CLASS(GraphUriBuilder_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto instance = ref new FacebookSDK::GraphUriBuilder(L"http://www.coderox.se/");

			// act

			// assert
			Assert::IsNotNull(instance);
		}
	};
}
