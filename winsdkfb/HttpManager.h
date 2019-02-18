#pragma once

#include "HttpManager.g.h"

namespace winrt::winsdkfb::implementation
{
	struct HttpManager : HttpManagerT<HttpManager>
	{
		HttpManager() = delete;

		void SetHttpClient(winsdkfb::IHttpClient const& httpClient);
		Windows::Foundation::IAsyncOperation<hstring> GetTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		Windows::Foundation::IAsyncOperation<hstring> PostTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		Windows::Foundation::IAsyncOperation<hstring> DeleteTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		hstring ParametersToQueryString(Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const& parameters);

		static winsdkfb::HttpManager Instance();

	private:
		static HttpManager _instance;
		HttpManager(winsdkfb::IHttpClient const& httpClient);
		winsdkfb::IHttpClient _httpClient;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct HttpManager : HttpManagerT<HttpManager, implementation::HttpManager>
	{
	};
}
