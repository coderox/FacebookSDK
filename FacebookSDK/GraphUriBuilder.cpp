#include "pch.h"
#include "GraphUriBuilder.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::FacebookSDK::implementation
{
	GraphUriBuilder::GraphUriBuilder(hstring const& path)
	{
		throw hresult_not_implemented();
	}

	Uri GraphUriBuilder::MakeUri()
	{
		throw hresult_not_implemented();
	}

	void GraphUriBuilder::AddQueryParam(hstring const& query, hstring const& param)
	{
		throw hresult_not_implemented();
	}

	void GraphUriBuilder::BuildApiVersionString() {
	
	}
	
	void GraphUriBuilder::FixPathDelimiters() { 
	
	}

	void GraphUriBuilder::DecodeQueryParams(Uri const& uri) {

	}

}
