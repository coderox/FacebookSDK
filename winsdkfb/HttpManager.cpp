﻿#include "pch.h"
#include "HttpManager.h"
#include "FBClient.h"

namespace winrt::winsdkfb::implementation
{
	HttpManager::HttpManager(winsdkfb::IHttpClient const& httpClient)
	{
		_httpClient = httpClient;
	}

	void HttpManager::SetHttpClient(winsdkfb::IHttpClient const& httpClient)
	{
		_httpClient = httpClient;
	}

	Windows::Foundation::IAsyncOperation<hstring> HttpManager::GetTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters)
	{
		return _httpClient.GetTaskAsync(path, parameters);
	}

	Windows::Foundation::IAsyncOperation<hstring> HttpManager::PostTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters)
	{
		return _httpClient.PostTaskAsync(path, parameters);
	}

	Windows::Foundation::IAsyncOperation<hstring> HttpManager::DeleteTaskAsync(hstring const path, Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const parameters)
	{
		return _httpClient.DeleteTaskAsync(path, parameters);
	}

	hstring HttpManager::ParametersToQueryString(Windows::Foundation::Collections::IMapView<hstring, Windows::Foundation::IInspectable> const& parameters)
	{
		return _httpClient.ParametersToQueryString(parameters);
	}

	winsdkfb::HttpManager HttpManager::Instance()
	{
		static HttpManager _instance(winrt::make<FBClient>());
		return _instance;
	}
}
