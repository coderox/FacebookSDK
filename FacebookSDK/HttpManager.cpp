#include "HttpManager.h"
#include "FBClient.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
{
	HttpManager::HttpManager(shared_ptr<IHttpClient> httpClient)
	{
		_httpClient = httpClient;
	}

	void HttpManager::SetHttpClient(shared_ptr<IHttpClient> httpClient)
	{
		_httpClient = httpClient;
	}

	IAsyncOperation<hstring> HttpManager::GetTaskAsync(hstring const path, IMapView<hstring, IInspectable> parameters)
	{
		return _httpClient->GetTaskAsync(path, parameters);
	}

	IAsyncOperation<hstring> HttpManager::PostTaskAsync(hstring const path, IMapView<hstring, IInspectable> const parameters)
	{
		return _httpClient->PostTaskAsync(path, parameters);
	}

	IAsyncOperation<hstring> HttpManager::DeleteTaskAsync(hstring const path, IMapView<hstring, IInspectable> const parameters)
	{
		return _httpClient->DeleteTaskAsync(path, parameters);
	}

	hstring HttpManager::ParametersToQueryString(IMapView<hstring, IInspectable> const parameters)
	{
		return _httpClient->ParametersToQueryString(parameters);
	}

	std::shared_ptr<HttpManager> HttpManager::Instance()
	{
		static std::shared_ptr<HttpManager> _instance = std::make_shared<HttpManager>(std::make_shared<FBClient>());
		return _instance;
	}
}
