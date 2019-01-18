#pragma once

#include "FacebookPaginatedArray.g.h"
#include "Generated/Graph.FBPaging.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookPaginatedArray : FacebookPaginatedArrayT<FacebookPaginatedArray>
	{
		FacebookPaginatedArray() = delete;
		FacebookPaginatedArray(hstring const& Request, Windows::Foundation::Collections::PropertySet const& Parameters, FacebookSDK::JsonClassFactory const& ObjectFactory);

		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FirstAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> NextAsync();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> PreviousAsync();
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> Current();
		hstring CurrentDataString();
		bool HasCurrent();
		bool HasNext();
		bool HasPrevious();
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> ObjectArrayFromWebResponse(hstring const& Response, FacebookSDK::JsonClassFactory const& classFactory);

	private:
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable>
			ObjectArrayFromJsonArray(
				Windows::Data::Json::JsonArray values,
				FacebookSDK::JsonClassFactory classFactory
			);

		FacebookSDK::FacebookResult ConsumePagedResponse(
			std::wstring jsonText
		);

		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> GetPageAsync(
			std::wstring path
		);

		std::wstring _currentDataString;
		std::wstring _request;
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> _current{ nullptr };
		FacebookSDK::Graph::FBPaging _paging{ nullptr };
		Windows::Foundation::Collections::PropertySet _parameters = nullptr;
		FacebookSDK::JsonClassFactory _objectFactory = nullptr;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookPaginatedArray : FacebookPaginatedArrayT<FacebookPaginatedArray, implementation::FacebookPaginatedArray>
	{
	};
}
