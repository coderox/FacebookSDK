#pragma once
#include <winrt/Windows.Foundation.Collections.h>

namespace FacebookSDK {
	struct IHttpClient {
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(
			winrt::hstring path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
			winrt::hstring path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
			winrt::hstring path,
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
		);

		winrt::hstring ParametersToQueryString(
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
		);
	};
}
