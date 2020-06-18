#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <map>

namespace winsdkfb 
{
	struct IHttpClient
	{
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(
			winrt::hstring const path,
			std::unordered_map<winrt::hstring, winrt::hstring> const parameters
		) = 0;

		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
			winrt::hstring const path,
			std::unordered_map<winrt::hstring, winrt::hstring> const parameters
		) = 0;

		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
			winrt::hstring const path,
			std::unordered_map<winrt::hstring, winrt::hstring> const parameters
		) = 0;

		virtual winrt::hstring ParametersToQueryString(
			std::unordered_map<winrt::hstring, winrt::hstring> const parameters
		) = 0;
	};
}
