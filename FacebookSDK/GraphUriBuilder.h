#pragma once

#include "GraphUriBuilder.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct GraphUriBuilder : GraphUriBuilderT<GraphUriBuilder>
	{
		GraphUriBuilder() = delete;
		GraphUriBuilder(hstring const& path);

		Windows::Foundation::Uri MakeUri();
		void AddQueryParam(hstring const& query, hstring const& param);

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

namespace winrt::FacebookSDK::factory_implementation
{
	struct GraphUriBuilder : GraphUriBuilderT<GraphUriBuilder, implementation::GraphUriBuilder>
	{
	};
}
