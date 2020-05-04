#include "pch.h"
#include "MockHttpClient.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

IAsyncOperation<hstring> MockHttpClient::GetTaskAsync(hstring path, IMapView<hstring, IInspectable> parameters)
{
	co_return ResponseData();
}

IAsyncOperation<hstring> MockHttpClient::PostTaskAsync(
	hstring path, IMapView<hstring, IInspectable> parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

IAsyncOperation<hstring> MockHttpClient::DeleteTaskAsync(
	hstring path, IMapView<hstring, IInspectable> parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

hstring MockHttpClient::ParametersToQueryString(
	IMapView<hstring, IInspectable> parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}