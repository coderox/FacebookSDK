#pragma once

#include <pplawait.h>

namespace winsdkfb 
{
	struct IHttpClient
	{
		virtual ~IHttpClient() = default;
		virtual concurrency::task<std::wstring> GetTaskAsync(
			std::wstring const path,
			std::unordered_map<std::wstring, std::wstring> const parameters
		) = 0;

		virtual concurrency::task<std::wstring> PostTaskAsync(
			std::wstring const path,
			std::unordered_map<std::wstring, std::wstring> const parameters
		) = 0;

		virtual concurrency::task<std::wstring> DeleteTaskAsync(
			std::wstring const path,
			std::unordered_map<std::wstring, std::wstring> const parameters
		) = 0;

		virtual std::wstring ParametersToQueryString(
			std::unordered_map<std::wstring, std::wstring> const parameters
		) = 0;
	};
}
