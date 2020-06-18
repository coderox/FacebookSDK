#include "FBClient.h"
#include "FBSession.h"
#include "SDKMessage.h"
#include "FBConstants.h"
#include "GraphUriBuilder.h"

#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.Web.Http.Filters.h>
#include <winrt/Windows.Web.Http.Headers.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Threading;
using namespace Windows::Data::Json;
using namespace Windows::Web::Http::Filters;
using namespace Windows::Web::Http::Headers;

#define BufferSize (1024 * 4) // 4kb
#define UnknownResponse = L"Unknown facebook response."
#define ETagKey L"_etag_"
#define UserAgent L"User-Agent"
#define WinSDKFBUserAgent(version) L"FBWinSDK." version
#define WinSDKFBUserAgentString WinSDKFBUserAgent(WINSDKFB_VERSION)

namespace winsdkfb
{
	IAsyncOperation<hstring> FBClient::GetTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		Uri uri = FBClient::PrepareRequestUri(path, parameters);

		auto response = co_await FBClient::GetTaskInternalAsync(uri);

		if (FBClient::IsOAuthErrorResponse(response)) {
			auto sess = FBSession::ActiveSession();
			co_await sess->TryRefreshAccessTokenAsync();
			response = co_await FBClient::GetTaskInternalAsync(uri);
		}
		co_return response;
	}

	IAsyncOperation<hstring> FBClient::GetTaskInternalAsync(
		Uri const& RequestUri
	) {
		HttpBaseProtocolFilter filter;
		HttpClient httpClient(filter);
		httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);

		filter.CacheControl().ReadBehavior(HttpCacheReadBehavior::Default);
		hstring result;
		try {
			auto response = co_await httpClient.GetAsync(RequestUri);
			result = co_await TryReceiveHttpResponseAsync(response);
		}
		catch (hresult_error e) {
			OutputDebugString(e.message().data());
		}
		co_return result;
	}

	IAsyncOperation<hstring> FBClient::TryReceiveHttpResponseAsync(
		HttpResponseMessage const& responseMessage)
	{
		hstring result;
		try
		{
			if (responseMessage && responseMessage.IsSuccessStatusCode())
			{
				result = co_await responseMessage.Content().ReadAsStringAsync();
			}
		}
		catch (hresult_error e)
		{
			OutputDebugString(e.message().data());
		}
		co_return result;
	}

	IAsyncOperation<hstring> FBClient::PostTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		auto uri = FBClient::PrepareRequestUri(path, parameters);

		auto result = co_await FBClient::SimplePostAsync(path, parameters);

		co_return result;
	}

	IAsyncOperation<hstring> FBClient::DeleteTaskAsync(hstring const path, unordered_map<hstring, hstring> const parameters)
	{
		auto uri = FBClient::PrepareRequestUri(path, parameters);

		hstring result;
		hstring response = co_await FBClient::DeleteTaskInternalAsync(uri);

		if (FBClient::IsOAuthErrorResponse(response)) {
			auto sess = FBSession::ActiveSession();
			co_await sess->TryRefreshAccessTokenAsync();
			result = co_await FBClient::DeleteTaskInternalAsync(uri);
		}
		else {
			result = response;
		}

		co_return result;
	}

	hstring FBClient::ParametersToQueryString(unordered_map<hstring, hstring> const parameters)
	{
		hstring queryString(L"");

		// Add remaining parameters to query string.  Note that parameters that
		// do not need to be uploaded as multipart, i.e. any which is are not
		// binary data, are required to be in the query string, even for POST
		// requests!
		for (auto const& current : parameters)
		{
			auto key = Uri::EscapeComponent(current.first);
			auto value = Uri::EscapeComponent(current.second);

			if (queryString.size() > 0)
			{
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
	 * @exception Can throw any exception that is thrown by SimplePlostInternalAsync
	 */
	IAsyncOperation<hstring> FBClient::SimplePostAsync(hstring const& path, unordered_map<hstring, hstring> const parameters) {
		Uri uri = FBClient::PrepareRequestUri(path, parameters);

		hstring result;
		hstring response = co_await FBClient::SimplePostInternalAsync(uri);
		if (FBClient::IsOAuthErrorResponse(response)) {
			co_await FBSession::ActiveSession()->TryRefreshAccessTokenAsync();
			result = co_await FBClient::SimplePostInternalAsync(uri);
		}
		else {
			result = response;
		}

		co_return result;
	}

	/**
	 * Builds request URI.
	 * @param httpMethod Type of HTTP request to build URI for
	 * @param path Request path
	 * @param parameters Query parameters for the request
	 * @return Request URI
	 * @exception FailureExecption if FBMediaObject or FBMediaStream are
	 * attempting to be attached on non-POST requests.
	 * @exception InvalidArgumentException if httpMethod is POST and improperly formatted/empty media object is attached.
	 */
	Uri FBClient::PrepareRequestUri(
		hstring const& path,
		unordered_map<hstring, hstring> const parameters
	) {
		auto sess = FBSession::ActiveSession();
		GraphUriBuilder uriBuilder(path);

		if (parameters.find(L"access_token") == parameters.end() &&
			(sess->AccessTokenData().AccessToken().data() != nullptr) &&
			(sess->AccessTokenData().AccessToken().size() > 0))
		{
			uriBuilder.AddQueryParam(L"access_token", sess->AccessTokenData().AccessToken());
		}

		if (parameters.find(L"format") == parameters.end())
		{
			uriBuilder.AddQueryParam(L"format", L"json-strings");
		}

		// Add remaining parameters to query string.  Note that parameters that
		// do not need to be uploaded as multipart, i.e. any which is are not
		// binary data, are required to be in the query string, even for POST
		// requests!
		for (auto const& current : parameters)
		{
			uriBuilder.AddQueryParam(current.first, current.second);
		}

		return uriBuilder.MakeUri();
	}

	/**
	 * Checks if Response is an OAuth error response.
	 * @param Response response to check
	 * @return true if Response does indicate an OAuth error, false otherwise.
	 */
	bool FBClient::IsOAuthErrorResponse(hstring const& response)
	{
		auto err = FBError::FromJson(response);
		return (err.Code() == 190);
	}

	/**
	 * Performs the actual HTTP DELETE request.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	IAsyncOperation<hstring> FBClient::DeleteTaskInternalAsync(Uri const& RequestUri)
	{
		HttpBaseProtocolFilter filter;
		HttpClient httpClient(filter);
		httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);

		auto response = co_await httpClient.DeleteAsync(RequestUri);
		auto result = co_await TryReceiveHttpResponseAsync(response);
		co_return result;
	}

	/**
	 * Performs the actual HTTP POST request with no POST data.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	IAsyncOperation<hstring> FBClient::SimplePostInternalAsync(Uri const& RequestUri)
	{
		HttpBaseProtocolFilter filter;
		HttpClient httpClient(filter);
		httpClient.DefaultRequestHeaders().Append(UserAgent, WinSDKFBUserAgentString);
		OutputDebugString(RequestUri.AbsoluteUri().c_str());
		auto response = co_await httpClient.PostAsync(RequestUri, HttpStringContent(L""));
		auto result = co_await TryReceiveHttpResponseAsync(response);
		co_return result;
	}
}
