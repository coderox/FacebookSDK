#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FBMediaObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{
	TEST_CLASS(FBMediaObject_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto instance = ref new winsdkfb::FBMediaObject();
			
			// act

			// assert
			Assert::IsNotNull(instance);
		}

		TEST_METHOD(TestSetValueWithBytes)
		{
			// arrange
			auto instance = ref new winsdkfb::FBMediaObject();
			std::vector<unsigned char> vector = { 0,1,2,3 };
			auto array = ref new Platform::Array<unsigned char>(&vector[0], safe_cast<unsigned int>(vector.size()));
			
			// act
			auto secondInstance = instance->SetValue(array);
			auto resultValue = instance->GetValue();

			// assert
			Assert::IsNotNull(instance);
			Assert::IsNotNull(secondInstance);
			Assert::AreEqual(instance, secondInstance);
			for (unsigned int i = 0; i < array->Length; i++) {
				Assert::AreEqual(array[i], resultValue[i]);
			}
		}
	};
}
