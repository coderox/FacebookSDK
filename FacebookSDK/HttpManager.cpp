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

	concurrency::task<wstring> HttpManager::GetTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) const {
		return _httpClient->GetTaskAsync(path, parameters);
	}

	concurrency::task<wstring> HttpManager::PostTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) const {
		return _httpClient->PostTaskAsync(path, parameters);
	}

	concurrency::task<wstring> HttpManager::DeleteTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) const {
		return _httpClient->DeleteTaskAsync(path, parameters);
	}

	wstring HttpManager::ParametersToQueryString(unordered_map<wstring, wstring> const parameters) const {
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
