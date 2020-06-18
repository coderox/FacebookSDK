#pragma once

#include "IHttpClient.h"

struct MockHttpClient : public winsdkfb::IHttpClient {
public:

	winrt::hstring ResponseData() {
		return responseData;
	}
	void ResponseData(winrt::hstring value) {
		responseData = value;
	}

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(
		winrt::hstring const path,
		std::unordered_map<winrt::hstring, winrt::hstring> const parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
		winrt::hstring const path,
		std::unordered_map<winrt::hstring, winrt::hstring> const parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
		winrt::hstring const path,
		std::unordered_map<winrt::hstring, winrt::hstring> const parameters
	);

	virtual winrt::hstring ParametersToQueryString(
		std::unordered_map<winrt::hstring, winrt::hstring> const parameters
	);

private:
	winrt::hstring responseData;
};