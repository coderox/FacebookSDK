#pragma once

#include "HttpManager.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct HttpManager : HttpManagerT<HttpManager>
	{
		HttpManager() = delete;

		void SetHttpClient(FacebookSDK::IHttpClient const& httpClient);
		Windows::Foundation::IAsyncOperation<hstring> GetTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		Windows::Foundation::IAsyncOperation<hstring> PostTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		Windows::Foundation::IAsyncOperation<hstring> DeleteTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters);
		hstring ParametersToQueryString(Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const& parameters);

		static FacebookSDK::HttpManager Instance();

	private:
		static HttpManager _instance;
		HttpManager(FacebookSDK::IHttpClient const& httpClient);
		FacebookSDK::IHttpClient _httpClient;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct HttpManager : HttpManagerT<HttpManager, implementation::HttpManager>
	{
	};
}
