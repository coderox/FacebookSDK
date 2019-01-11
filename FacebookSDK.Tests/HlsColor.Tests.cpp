#include "pch.h"
#include "CppUnitTest.h"
#include "../FacebookSDK/HlsColor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
	TEST_CLASS(HlsColors_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto hlsColor = ref new FacebookSDK::HlsColor();

			// act

			// assert
			Assert::IsNotNull(hlsColor);
		}

		TEST_METHOD(TestColorRedHasLuminosity)
		{
			// arrange
			auto hlsColor = ref new FacebookSDK::HlsColor();

			// act
			hlsColor->RgbValue = Windows::UI::Colors::Red;

			// assert
			Assert::AreEqual(0.5, hlsColor->Luminosity);
		}
	};
}
