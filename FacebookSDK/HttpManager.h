#pragma once

#include <IHttpClient.h>

namespace winsdkfb
{
	struct HttpManager
	{
		HttpManager() = delete;
		HttpManager(std::shared_ptr<winsdkfb::IHttpClient> httpClient);

		void SetHttpClient(std::shared_ptr<winsdkfb::IHttpClient> httpClient);
		concurrency::task<std::wstring> GetTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) const;
		concurrency::task<std::wstring> PostTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) const;
		concurrency::task<std::wstring> DeleteTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) const;
		std::wstring ParametersToQueryString(std::unordered_map<std::wstring, std::wstring> const parameters) const;

		static HttpManager* Instance();
		static void TearDown();

	private:
		static std::unique_ptr<HttpManager> _instance;
		std::shared_ptr<winsdkfb::IHttpClient> _httpClient;
	};
}
