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
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct GraphUriBuilder : GraphUriBuilderT<GraphUriBuilder, implementation::GraphUriBuilder>
	{
	};
}
