#pragma once

#include "IHttpClient.h"

struct MockHttpClient : public winsdkfb::IHttpClient {
public:
																																																							
	std::wstring ResponseData() {
		return responseData; 
	}
	void ResponseData(std::wstring value) {
		responseData = value; 
	}
	
	concurrency::task<std::wstring> GetTaskAsync(
		std::wstring path,
		std::unordered_map<std::wstring, std::wstring> const parameters
	) override;

	concurrency::task<std::wstring> PostTaskAsync(
		std::wstring path,
		std::unordered_map<std::wstring, std::wstring> const parameters
	) override;

	concurrency::task<std::wstring> DeleteTaskAsync(
		std::wstring path,
		std::unordered_map<std::wstring, std::wstring> const parameters
	) override;

	virtual std::wstring ParametersToQueryString(
		std::unordered_map<std::wstring, std::wstring> const parameters
	) override;

private:
	std::wstring responseData;
};