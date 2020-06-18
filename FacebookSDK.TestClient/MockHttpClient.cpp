#include "pch.h"
#include "MockHttpClient.h"
#include <map>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;

IAsyncOperation<hstring> MockHttpClient::GetTaskAsync(hstring path, unordered_map<hstring, hstring> const parameters)
{
	co_return ResponseData();
}

IAsyncOperation<hstring> MockHttpClient::PostTaskAsync(
	hstring path, unordered_map<hstring, hstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

IAsyncOperation<hstring> MockHttpClient::DeleteTaskAsync(
	hstring path, unordered_map<hstring, hstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

hstring MockHttpClient::ParametersToQueryString(
	unordered_map<hstring, hstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}