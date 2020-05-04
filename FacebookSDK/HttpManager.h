#pragma once

#include <IHttpClient.h>

namespace winsdkfb
{
	struct HttpManager
	{
		HttpManager() = delete;
		HttpManager(std::shared_ptr<winsdkfb::IHttpClient> httpClient);

		void SetHttpClient(std::shared_ptr<winsdkfb::IHttpClient> httpClient);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		winrt::hstring ParametersToQueryString(winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);

		static std::shared_ptr<winsdkfb::HttpManager> Instance();

	private:
		static std::shared_ptr<HttpManager> _instance;
		std::shared_ptr<winsdkfb::IHttpClient> _httpClient;
	};
}
