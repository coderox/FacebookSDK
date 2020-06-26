#pragma once
#include <string>
#include <ctime>
#include <functional>
#include <winrt/Windows.Foundation.Collections.h>
#undef X64
#include <winrt/Windows.Security.Authentication.Web.Core.h>

#include "FBResult.h"

uint64_t getUnixTimeStamp(const std::time_t* t = nullptr);
int compare_ordinal(std::wstring strA, std::wstring strB);
unsigned WindowsTickToUnixSeconds(long long windowsTicks);
std::wstring WebTokenRequestStatusToString(winrt::Windows::Security::Authentication::Web::Core::WebTokenRequestStatus status);

namespace winsdkfb {
typedef std::function<winrt::Windows::Foundation::Uri(std::unordered_map<std::wstring, std::wstring>)> DialogUriBuilder;

	typedef std::function<winsdkfb::FBResult(std::wstring)> JsonClassFactory;
}