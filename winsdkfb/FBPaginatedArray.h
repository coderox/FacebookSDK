#pragma once

#include "FBPaginatedArray.g.h"
#include "Generated/Graph.FBPaging.h"

namespace winrt::winsdkfb::implementation
{
	struct FBPaginatedArray : FBPaginatedArrayT<FBPaginatedArray>
	{
		FBPaginatedArray() = delete;
		FBPaginatedArray(hstring const& Request, Windows::Foundation::Collections::PropertySet const& Parameters, winsdkfb::JsonClassFactory const& ObjectFactory);

		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> FirstAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> NextAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> PreviousAsync();
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> Current();
		hstring CurrentDataString();
		bool HasCurrent();
		bool HasNext();
		bool HasPrevious();
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> ObjectArrayFromWebResponse(hstring const& Response, winsdkfb::JsonClassFactory const& classFactory);

	private:
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable>
			ObjectArrayFromJsonArray(
				Windows::Data::Json::JsonArray values,
				winsdkfb::JsonClassFactory classFactory
			);

		winsdkfb::FBResult ConsumePagedResponse(
			std::wstring jsonText
		);

		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetPageAsync(
			std::wstring path
		);

		std::wstring _currentDataString;
		std::wstring _request;
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> _current{ nullptr };
		winsdkfb::Graph::FBPaging _paging{ nullptr };
		Windows::Foundation::Collections::PropertySet _parameters = nullptr;
		winsdkfb::JsonClassFactory _objectFactory = nullptr;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBPaginatedArray : FBPaginatedArrayT<FBPaginatedArray, implementation::FBPaginatedArray>
	{
	};
}
