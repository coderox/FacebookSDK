#pragma once

#include <IHttpClient.h>

namespace winsdkfb
{
	struct HttpManager
	{
		HttpManager() = delete;
		HttpManager(std::shared_ptr<winsdkfb::IHttpClient> httpClient);

		void SetHttpClient(std::shared_ptr<winsdkfb::IHttpClient> httpClient);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		winrt::hstring ParametersToQueryString(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);

		static HttpManager* Instance();
		static void TearDown();

	private:
		static std::unique_ptr<HttpManager> _instance;
		std::shared_ptr<winsdkfb::IHttpClient> _httpClient;
	};
}
