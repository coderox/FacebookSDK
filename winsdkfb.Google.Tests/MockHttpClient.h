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
		winrt::hstring path, 
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
		winrt::hstring path,
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
		winrt::hstring path,
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
	);

	virtual winrt::hstring ParametersToQueryString(
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> parameters
	);

private:
	winrt::hstring responseData;
};