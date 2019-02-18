#pragma once

#include "FacebookPaginatedArray.g.h"
#include "Generated/Graph.FBPaging.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookPaginatedArray : FacebookPaginatedArrayT<FacebookPaginatedArray>
	{
		FacebookPaginatedArray() = delete;
		FacebookPaginatedArray(hstring const& Request, Windows::Foundation::Collections::PropertySet const& Parameters, winsdkfb::JsonClassFactory const& ObjectFactory);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> FirstAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> NextAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> PreviousAsync();
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

		winsdkfb::FacebookResult ConsumePagedResponse(
			std::wstring jsonText
		);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> GetPageAsync(
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
	struct FacebookPaginatedArray : FacebookPaginatedArrayT<FacebookPaginatedArray, implementation::FacebookPaginatedArray>
	{
	};
}
