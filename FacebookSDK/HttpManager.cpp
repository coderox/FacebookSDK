#include "HttpManager.h"
#include "FBClient.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
{
	std::unique_ptr<HttpManager> HttpManager::_instance;

	HttpManager::HttpManager(shared_ptr<IHttpClient> httpClient)
	{
		SetHttpClient(httpClient);
	}

	void HttpManager::SetHttpClient(shared_ptr<IHttpClient> httpClient)
	{
		_httpClient = httpClient;
	}

	IAsyncOperation<hstring> HttpManager::GetTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		return _httpClient->GetTaskAsync(path, parameters);
	}

	IAsyncOperation<hstring> HttpManager::PostTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		return _httpClient->PostTaskAsync(path, parameters);
	}

	IAsyncOperation<hstring> HttpManager::DeleteTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		return _httpClient->DeleteTaskAsync(path, parameters);
	}

	hstring HttpManager::ParametersToQueryString(unordered_map<hstring, hstring> const parameters)
	{
		return _httpClient->ParametersToQueryString(parameters);
	}

	HttpManager* HttpManager::Instance()
	{
		if (_instance == nullptr) {
			_instance.reset(new HttpManager(std::make_shared<FBClient>()));
		}
		return _instance.get();
	}

	void HttpManager::TearDown() {
		_instance.reset();
	}
}
