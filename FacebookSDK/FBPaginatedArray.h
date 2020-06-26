#pragma once

#include "Graph/FBPaging.h"
#include "FBResult.h"
#include "Utilities.h"

#include <winrt/Windows.Data.Json.h>

#include <pplawait.h>
#include <vector>

namespace winsdkfb
{
	struct FBPaginatedArray
	{
		FBPaginatedArray() = delete;
		FBPaginatedArray(std::wstring const& Request, std::unordered_map<std::wstring, std::wstring> parameters, winsdkfb::JsonClassFactory const& ObjectFactory);

		concurrency::task<winsdkfb::FBResult> FirstAsync();
		concurrency::task<winsdkfb::FBResult> NextAsync();
		concurrency::task<winsdkfb::FBResult> PreviousAsync();
		std::vector<winsdkfb::FBResult> Current() const;
		std::wstring CurrentDataString() const;
		bool HasCurrent() const;
		bool HasNext() const;
		bool HasPrevious() const;
		static std::vector<winsdkfb::FBResult> VectorFromWebResponse(std::wstring const& Response, winsdkfb::JsonClassFactory const& classFactory);

	private:
		static std::vector<winsdkfb::FBResult>
			VectorFromJsonArray(
				winrt::Windows::Data::Json::JsonArray values,
				winsdkfb::JsonClassFactory classFactory
			);

		winsdkfb::FBResult ConsumePagedResponse(
			std::wstring jsonText
		);

		concurrency::task<winsdkfb::FBResult> GetPageAsync(
			std::wstring path
		);

		std::wstring _currentDataString;
		std::wstring _request;
		std::vector<winsdkfb::FBResult> _current;
		winsdkfb::Graph::FBPaging _paging;
		std::unordered_map<std::wstring, std::wstring> _parameters;
		winsdkfb::JsonClassFactory _objectFactory;
	};
}
