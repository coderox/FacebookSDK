#include "FBClient.h"
#include "FBSession.h"
#include "FBConstants.h"
#include "GraphUriBuilder.h"

#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.Web.Http.Filters.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <pplawait.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Collections;
using namespace Windows::Web::Http;
using namespace Filters;
using namespace Headers;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Threading;
using namespace Windows::Data::Json;
using namespace concurrency;

#define BufferSize (1024 * 4) // 4kb
#define UnknownResponse = L"Unknown facebook response."
#define ETagKey L"_etag_"
#define UserAgent L"User-Agent"
#define WinSDKFBUserAgent(version) L"FBWinSDK." version
#define WinSDKFBUserAgentString WinSDKFBUserAgent(WINSDKFB_VERSION)

namespace winsdkfb {
task<wstring> FBClient::GetTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) {
	const auto uri = PrepareRequestUri(path, parameters);

	auto response = co_await GetTaskInternalAsync(uri);

	if (IsOAuthErrorResponse(response)) {
		const auto session = FBSession::ActiveSession();
		co_await session->TryRefreshAccessTokenAsync();
		response = co_await GetTaskInternalAsync(uri);
	}
	co_return response;
}

task<wstring> FBClient::GetTaskInternalAsync(
	Uri const& RequestUri
	) {
	const HttpBaseProtocolFilter filter;
	const HttpClient httpClient(filter);
	httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);

	filter.CacheControl().ReadBehavior(HttpCacheReadBehavior::Default);
	wstring result;
	try {
		const auto response = co_await httpClient.GetAsync(RequestUri);
		result = co_await TryReceiveHttpResponseAsync(response);
	} catch (hresult_error e) {
		OutputDebugString(e.message().data());
	}
	co_return result;
}

task<wstring> FBClient::TryReceiveHttpResponseAsync(
	HttpResponseMessage const& responseMessage) {
	wstring result;
	try {
		if (responseMessage && responseMessage.IsSuccessStatusCode()) {
			result = co_await responseMessage.Content().ReadAsStringAsync();
		}
	} catch (hresult_error e) {
		OutputDebugString(e.message().data());
	}
	co_return result;
}

task<wstring> FBClient::PostTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) {
	auto uri = PrepareRequestUri(path, parameters);

	auto result = co_await SimplePostAsync(path, parameters);

	co_return result;
}

task<wstring> FBClient::DeleteTaskAsync(wstring const path, unordered_map<wstring, wstring> const parameters) {
	const auto uri = PrepareRequestUri(path, parameters);

	wstring result;
	const auto response = co_await DeleteTaskInternalAsync(uri);

	if (IsOAuthErrorResponse(response)) {
		const auto session = FBSession::ActiveSession();
		co_await session->TryRefreshAccessTokenAsync();
		result = co_await DeleteTaskInternalAsync(uri);
	} else {
		result = response;
	}

	co_return result;
}

wstring FBClient::ParametersToQueryString(unordered_map<wstring, wstring> const parameters) {
	wstring queryString(L"");

	// Add remaining parameters to query string.  Note that parameters that
	// do not need to be uploaded as multipart, i.e. any which is are not
	// binary data, are required to be in the query string, even for POST
	// requests!
	for (auto const& current : parameters) {
		auto key = Uri::EscapeComponent(current.first);
		auto value = Uri::EscapeComponent(current.second);

		if (queryString.size() > 0) {
			queryString = queryString + L"&";
		}

		queryString = queryString + key + L"=" + value;
	}

	return queryString;
}

/**
 * Non-stream HTTP POST flow.
 * @param path URL to send POST request to
 * @param parameters query parameters to attach to POST request
 * @return IAsyncOperation containing the response content
 * @exception Can throw any exception that is thrown by SimplePostInternalAsync
 */
task<wstring> FBClient::SimplePostAsync(wstring const& path, unordered_map<wstring, wstring> const parameters) {
	const auto uri = PrepareRequestUri(path, parameters);

	wstring result;
	const auto response = co_await SimplePostInternalAsync(uri);
	if (IsOAuthErrorResponse(response)) {
		co_await FBSession::ActiveSession()->TryRefreshAccessTokenAsync();
		result = co_await SimplePostInternalAsync(uri);
	} else {
		result = response;
	}

	co_return result;
}

/**
 * Builds request URI.
 * @param path Request path
 * @param parameters Query parameters for the request
 * @return Request URI
 * @exception FailureExecption if FBMediaObject or FBMediaStream are
 * attempting to be attached on non-POST requests.
 * @exception InvalidArgumentException if httpMethod is POST and improperly formatted/empty media object is attached.
 */
Uri FBClient::PrepareRequestUri(
	wstring const& path,
	unordered_map<wstring, wstring> const& parameters
	) {
	const auto session = FBSession::ActiveSession();
	GraphUriBuilder uriBuilder(path);

	if (parameters.find(L"access_token") == parameters.end() &&
		(session->AccessTokenData().AccessToken().data() != nullptr) &&
		(session->AccessTokenData().AccessToken().size() > 0)) {
		uriBuilder.AddQueryParam(L"access_token", session->AccessTokenData().AccessToken());
	}

	if (parameters.find(L"format") == parameters.end()) {
		uriBuilder.AddQueryParam(L"format", L"json-strings");
	}

	// Add remaining parameters to query string.  Note that parameters that
	// do not need to be uploaded as multipart, i.e. any which is are not
	// binary data, are required to be in the query string, even for POST
	// requests!
	for (auto const& current : parameters) {
		uriBuilder.AddQueryParam(current.first, current.second);
	}

	return uriBuilder.MakeUri();
}

/**
 * Checks if Response is an OAuth error response.
 * @param response response to check
 * @return true if Response does indicate an OAuth error, false otherwise.
 */
bool FBClient::IsOAuthErrorResponse(wstring const& response) {
	const auto err = FBError::FromJson(response);
	return (err.Code() == 190);
}

/**
 * Performs the actual HTTP DELETE request.
 * @param RequestUri the full URI of the request
 * @return The response content
 * @exception Exception Any exception that can occur during the request
 */
task<wstring> FBClient::DeleteTaskInternalAsync(Uri const& RequestUri) {
	const HttpBaseProtocolFilter filter;
	const HttpClient httpClient(filter);
	httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);

	const auto response = co_await httpClient.DeleteAsync(RequestUri);
	const auto result = co_await TryReceiveHttpResponseAsync(response);
	co_return result.c_str();
}

/**
 * Performs the actual HTTP POST request with no POST data.
 * @param RequestUri the full URI of the request
 * @return The response content
 * @exception Exception Any exception that can occur during the request
 */
task<wstring> FBClient::SimplePostInternalAsync(Uri const& RequestUri) {
	const HttpBaseProtocolFilter filter;
	const HttpClient httpClient(filter);
	httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);
	OutputDebugString(RequestUri.AbsoluteUri().c_str());
	const auto response = co_await httpClient.PostAsync(RequestUri, HttpStringContent(L""));
	const auto result = co_await TryReceiveHttpResponseAsync(response);
	co_return result.c_str();
}
}
