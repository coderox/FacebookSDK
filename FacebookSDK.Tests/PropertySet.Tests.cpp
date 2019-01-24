#include "pch.h"
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include "CppUnitTest.h"
#include <ctime>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation::Collections;
namespace FacebookSDK_Tests
{

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

	TEST_CLASS(PropertySet_Tests)
	{	
	public:
		TEST_METHOD(TestInsertOneRowIntoPropertySet)
		{
			// arrange
			PropertySet^ parameters = ref new PropertySet();

			// act
			auto values = ref new Platform::String(L"gender,link,first_name,last_name,locale,timezone,email,updated_time,verified,name,id,picture");
			parameters->Insert(L"fields", dynamic_cast<Platform::Object^>(values));

			// assert
			Assert::AreEqual(1, (int)parameters->Size);
		}
	};
}