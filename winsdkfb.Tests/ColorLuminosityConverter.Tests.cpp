#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/ColorLuminosityConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(ColorLuminosityConverter_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto converter = ref new winsdkfb::ColorLuminosityConverter();

			// act

			// assert
			Assert::IsNotNull(converter);
		}

		TEST_METHOD(TestConvert)
		{
			// arrange
			auto converter = ref new winsdkfb::ColorLuminosityConverter();

			// act
			auto oldColor = Windows::UI::Colors::Red;
			auto color = converter->ApplyFactor(oldColor, 0.7);

			// assert
			Assert::AreEqual(178, (int)color.R);
		}
	};
}
