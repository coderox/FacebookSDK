#pragma once
#include <string>
#include <ctime>
#include <winrt/Windows.Security.Authentication.Web.Core.h>

uint64_t getUnixTimeStamp(const std::time_t* t = nullptr);
int compare_ordinal(std::wstring strA, std::wstring strB);
unsigned WindowsTickToUnixSeconds(long long windowsTicks);
winrt::hstring WebTokenRequestStatusToString(winrt::Windows::Security::Authentication::Web::Core::WebTokenRequestStatus status);

namespace winsdkfb {
	struct DialogUriBuilder
	{
		winrt::Windows::Foundation::Uri operator()(winrt::Windows::Foundation::Collections::PropertySet const& d);
	};

	struct JsonClassFactory
	{
		winrt::Windows::Foundation::IInspectable operator()(winrt::hstring const& d);
	};
}