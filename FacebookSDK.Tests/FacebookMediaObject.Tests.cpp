#include "pch.h"
#include "CppUnitTest.h"
#include "../FacebookSDK/FacebookMediaObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
	TEST_CLASS(FacebookMediaObject_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto instance = ref new FacebookSDK::FacebookMediaObject();
			
			// act

			// assert
			Assert::IsNotNull(instance);
		}

		TEST_METHOD(TestSetValueWithBytes)
		{
			// arrange
			auto instance = ref new FacebookSDK::FacebookMediaObject();
			std::vector<unsigned char> vector = { 0,1,2,3 };
			auto array = ref new Platform::Array<unsigned char>(&vector[0], vector.size());
			
			// act
			auto secondInstance = instance->SetValue(array);
			auto resultValue = instance->GetValue();

			// assert
			Assert::IsNotNull(instance);
			Assert::IsNotNull(secondInstance);
			Assert::AreEqual(instance, secondInstance);
			for (int i = 0; i < array->Length; i++) {
				Assert::AreEqual(array[i], resultValue[i]);
			}
		}
	};
}
