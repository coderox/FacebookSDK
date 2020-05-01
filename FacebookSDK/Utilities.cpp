#include "pch.h"
#include "Utilities.h"
#include <chrono>

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

using namespace winrt;
using namespace Windows::Security::Authentication::Web::Core;

uint64_t getUnixTimeStamp(const std::time_t* t)
{
	//if specific time is not passed then get current time
	std::time_t st = t == nullptr ? std::time(nullptr) : *t;
	auto secs = static_cast<std::chrono::seconds>(st).count();
	return static_cast<uint64_t>(secs);
}

int compare_ordinal(std::wstring strA, std::wstring strB) {
	std::transform(strA.begin(), strA.end(), strA.begin(), ::towlower);
	std::transform(strB.begin(), strB.end(), strB.begin(), ::towlower);
	return strA.compare(strB);
}

unsigned WindowsTickToUnixSeconds(long long windowsTicks) {
	return (unsigned)(windowsTicks / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

hstring WebTokenRequestStatusToString(WebTokenRequestStatus status) {
	switch (status)
	{
	case WebTokenRequestStatus::Success:
		return L"Success";
	case WebTokenRequestStatus::UserCancel:
		return L"UserCanel";
	case WebTokenRequestStatus::UserInteractionRequired:
		return L"UserInteractionRequired";
	case WebTokenRequestStatus::AccountProviderNotAvailable:
		return L"AccountProviderNotAvailable";
	case WebTokenRequestStatus::AccountSwitch:
		return L"AccountSwitch";
	case WebTokenRequestStatus::ProviderError:
		return L"ProviderError";
	default:
		return L"Unknown";
	}
}
