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
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(
		winrt::hstring const path,
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
	);

	virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(
		winrt::hstring const path,
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
	);

	virtual winrt::hstring ParametersToQueryString(
		winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters
	);

private:
	winrt::hstring responseData;
};