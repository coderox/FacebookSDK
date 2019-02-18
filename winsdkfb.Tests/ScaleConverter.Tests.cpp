#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/ScaleConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(ScaleConverter_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto converter = ref new winsdkfb::ScaleConverter();

			// act

			// assert
			Assert::IsNotNull(converter);
		}

		TEST_METHOD(TestConvertSuccess)
		{
			// arrange
			auto converter = ref new winsdkfb::ScaleConverter();

			// act
			auto typeName = Windows::UI::Xaml::Interop::TypeName();
			auto value = 100.0;
			typeName.Name = value.GetType()->FullName;
			auto result = safe_cast<double>(converter->Convert(value, typeName, "0.7", ref new Platform::String(L"en")));

			// assert
			Assert::AreEqual(70.0, result);
		}

		TEST_METHOD(TestConvertFail)
		{
			// arrange
			auto converter = ref new winsdkfb::ScaleConverter();

			// act
			try {
				auto typeName = Windows::UI::Xaml::Interop::TypeName();
				auto value = 100;
				typeName.Name = value.GetType()->FullName;
				auto result = safe_cast<double>(converter->Convert(value, typeName, "0.7", ref new Platform::String(L"en")));
				Assert::Fail(L"No exception thrown", LINE_INFO());
			}
			catch (Platform::InvalidArgumentException^ exception) {
				// assert
			}
			catch (...) {
				// assert
				Assert::Fail(L"Incorrect exception thrown", LINE_INFO());
			}
		}
	};
}
