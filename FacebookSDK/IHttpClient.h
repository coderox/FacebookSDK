#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb 
{
	struct IHttpClient
	{
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(
			winrt::hstring const path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
		) = 0;

		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
			winrt::hstring const path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
		) = 0;

		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
			winrt::hstring const path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
		) = 0;

		virtual winrt::hstring ParametersToQueryString(
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
		) = 0;
	};
}
