#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct GraphUriBuilder
	{
		GraphUriBuilder() = delete;
		GraphUriBuilder(winrt::hstring const& path);

		winrt::Windows::Foundation::Uri MakeUri();
		void AddQueryParam(winrt::hstring const& query, winrt::hstring const& param);

	private:
		void BuildApiVersionString();
		void FixPathDelimiters();
		void DecodeQueryParams(winrt::Windows::Foundation::Uri const& uri);

		std::wstring _host;
		std::wstring _path;
		std::wstring _scheme;
		std::wstring _apiVersion;
		winrt::Windows::Foundation::Collections::PropertySet _queryParams;
	};
}
