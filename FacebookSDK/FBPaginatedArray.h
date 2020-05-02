#pragma once

#include "Graph/FBPaging.h"
#include "FBResult.h"
#include <winrt/Windows.Foundation.Collections.h>
#include "Utilities.h"

namespace winsdkfb
{
	struct FBPaginatedArray
	{
		FBPaginatedArray() = delete;
		FBPaginatedArray(winrt::hstring const& Request, winrt::Windows::Foundation::Collections::PropertySet const& Parameters, winsdkfb::JsonClassFactory const& ObjectFactory);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> FirstAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> NextAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> PreviousAsync();
		winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IInspectable> Current();
		winrt::hstring CurrentDataString();
		bool HasCurrent();
		bool HasNext();
		bool HasPrevious();
		winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IInspectable> ObjectArrayFromWebResponse(winrt::hstring const& Response, winsdkfb::JsonClassFactory const& classFactory);

	private:
		winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IInspectable>
			ObjectArrayFromJsonArray(
				winrt::Windows::Data::Json::JsonArray values,
				winsdkfb::JsonClassFactory classFactory
			);

		winsdkfb::FBResult ConsumePagedResponse(
			std::wstring jsonText
		);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetPageAsync(
			std::wstring path
		);

		std::wstring _currentDataString;
		std::wstring _request;
		winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IInspectable> _current{ nullptr };
		std::shared_ptr<winsdkfb::Graph::FBPaging> _paging{ nullptr };
		winrt::Windows::Foundation::Collections::PropertySet _parameters = nullptr;
		std::shared_ptr<winsdkfb::JsonClassFactory> _objectFactory = nullptr;
	};
}
