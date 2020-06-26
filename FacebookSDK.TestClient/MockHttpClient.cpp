#include "pch.h"
#include "MockHttpClient.h"
#include <map>

using namespace std;
using namespace winrt;
using namespace concurrency;

task<wstring> MockHttpClient::GetTaskAsync(wstring path, unordered_map<wstring, wstring> const parameters)
{
	co_return ResponseData();
}

task<wstring> MockHttpClient::PostTaskAsync(
	wstring path, unordered_map<wstring, wstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

task<wstring> MockHttpClient::DeleteTaskAsync(
	wstring path, unordered_map<wstring, wstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}

wstring MockHttpClient::ParametersToQueryString(
	unordered_map<wstring, wstring> const parameters
) {
	throw hresult_error(0, L"Not implemented yet");
}