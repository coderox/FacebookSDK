#pragma once

#include "Graph/FBPaging.h"
#include "FBResult.h"
#include "Utilities.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

#include <pplawait.h>
#include <vector>

namespace winsdkfb
{
	struct FBPaginatedArray
	{
		FBPaginatedArray() = delete;
		FBPaginatedArray(winrt::hstring const& Request, winrt::Windows::Foundation::Collections::PropertySet const& Parameters, winsdkfb::JsonClassFactory const& ObjectFactory);

		concurrency::task<winsdkfb::FBResult> FirstAsync();
		concurrency::task<winsdkfb::FBResult> NextAsync();
		concurrency::task<winsdkfb::FBResult> PreviousAsync();
		std::vector<winsdkfb::FBResult> Current();
		winrt::hstring CurrentDataString();
		bool HasCurrent();
		bool HasNext();
		bool HasPrevious();
		std::vector<winsdkfb::FBResult> VectorFromWebResponse(winrt::hstring const& Response, winsdkfb::JsonClassFactory const& classFactory);

	private:
		std::vector<winsdkfb::FBResult>
			VectorFromJsonArray(
				winrt::Windows::Data::Json::JsonArray values,
				winsdkfb::JsonClassFactory classFactory
			);

		winsdkfb::FBResult ConsumePagedResponse(
			std::wstring jsonText
		);

		concurrency::task<winsdkfb::FBResult> GetPageAsync(
			winrt::hstring path
		);

		winrt::hstring _currentDataString;
		winrt::hstring _request;
		std::vector<winsdkfb::FBResult> _current;
		winsdkfb::Graph::FBPaging _paging;
		winrt::Windows::Foundation::Collections::PropertySet _parameters = nullptr;
		winsdkfb::JsonClassFactory _objectFactory;
	};
}
