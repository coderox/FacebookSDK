#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct GraphUriBuilder
	{
		GraphUriBuilder() = delete;
		GraphUriBuilder(std::wstring const& path);

		winrt::Windows::Foundation::Uri MakeUri();
		void AddQueryParam(std::wstring const& query, std::wstring const& param) const;

	private:
		void BuildApiVersionString();
		void FixPathDelimiters();
		void DecodeQueryParams(winrt::Windows::Foundation::Uri const& uri) const;

		std::wstring _host;
		std::wstring _path;
		std::wstring _scheme;
		std::wstring _apiVersion;
		winrt::Windows::Foundation::Collections::PropertySet _queryParams;
	};
}
