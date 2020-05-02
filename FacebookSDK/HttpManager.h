#pragma once

#include <IHttpClient.h>

namespace winsdkfb
{
	struct HttpManager
	{
		HttpManager() = delete;

		void SetHttpClient(winsdkfb::IHttpClient const& httpClient);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::hstring ParametersToQueryString(winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const& parameters);

		static winsdkfb::HttpManager Instance();

	private:
		static HttpManager _instance;
		HttpManager(winsdkfb::IHttpClient const& httpClient);
		winsdkfb::IHttpClient _httpClient;
	};
}
