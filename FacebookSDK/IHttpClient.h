#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb 
{
	struct IHttpClient
	{
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(
			winrt::hstring const& path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring const&, winrt::Windows::Foundation::IInspectable const&> parameters
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
			winrt::hstring const& path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring const&, winrt::Windows::Foundation::IInspectable const&> parameters
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
			winrt::hstring const& path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring const&, winrt::Windows::Foundation::IInspectable const&> parameters
		);

		winrt::hstring ParametersToQueryString(
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring const&, winrt::Windows::Foundation::IInspectable const&> parameters
		);
	};
}
