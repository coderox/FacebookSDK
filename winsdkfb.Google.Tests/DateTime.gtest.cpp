#include "pch.h"

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>

#include <ctime>
#include <chrono>

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

//TEST(DateTimeTests, FromUnixEpoch)
//{
//	// arrange
//	auto value = L"1548064800";
//	winrt::hstring expirationString(value);
//	winrt::Windows::Foundation::DateTime cxExpirationTime;
//	winrt::Windows::Foundation::DateTime cppWinRTExpirationTime;
//
//	// act
//	cxExpirationTime.UniversalTime = _wtoi64(expirationString->Data());
//
//	//std::time_t t = getUnixTimeStamp(nullptr);
//	std::time_t t(1548064800);
//	//t += _wtoi64(expirationString->Data());
//	cppWinRTExpirationTime = winrt::clock::from_time_t(t);
//	auto count = WindowsTickToUnixSeconds(cppWinRTExpirationTime.time_since_epoch().count());
//
//	// assert
//	Assert::IsTrue(cxExpirationTime.UniversalTime == count);
//}

