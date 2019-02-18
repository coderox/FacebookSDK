#include "pch.h"
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include "CppUnitTest.h"
#include <ctime>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winsdkfb_Tests
{

	#define WINDOWS_TICK 10000000
	#define SEC_TO_UNIX_EPOCH 11644473600LL

	TEST_CLASS(DateTime_Tests)
	{
		unsigned WindowsTickToUnixSeconds(long long windowsTicks) {
			return (unsigned)(windowsTicks / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
		}

		static uint64_t getUnixTimeStamp(const std::time_t* t = nullptr)
		{
			//if specific time is not passed then get current time
			std::time_t st = t == nullptr ? std::time(nullptr) : *t;
			auto secs = static_cast<std::chrono::seconds>(st).count();
			return static_cast<uint64_t>(secs);
		}

	public:
		TEST_METHOD(TestDateTimeFromUnixEpoch)
		{
			// arrange
			auto value = L"1548064800";
			Platform::String^ expirationString = ref new Platform::String(value);
			Windows::Foundation::DateTime cxExpirationTime;
			winrt::Windows::Foundation::DateTime cppWinRTExpirationTime;

			// act
			cxExpirationTime.UniversalTime = _wtoi64(expirationString->Data());

			//std::time_t t = getUnixTimeStamp(nullptr);
			std::time_t t(1548064800);
			//t += _wtoi64(expirationString->Data());
			cppWinRTExpirationTime = winrt::clock::from_time_t(t);
			auto count = WindowsTickToUnixSeconds(cppWinRTExpirationTime.time_since_epoch().count());

			// assert
			Assert::IsTrue(cxExpirationTime.UniversalTime == count);
		}
	};
}