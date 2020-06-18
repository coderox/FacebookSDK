#pragma once

#include "HttpMethod.h"
#include "IHttpClient.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Http.h>

namespace winsdkfb
{
	struct FBClient : public IHttpClient
	{
		FBClient() = default;

		// IHttpClient
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(winrt::hstring const path, std::unordered_map<winrt::hstring, winrt::hstring> const parameters);
		virtual winrt::hstring ParametersToQueryString(std::unordered_map<winrt::hstring, winrt::hstring> const parameters);

	private:

		/**
		 * Non-stream HTTP POST flow.
		 * @param path URL to send POST request to
		 * @param parameters query parameters to attach to POST request
		 * @return IAsyncOperation containing the response content
		 * @exception Can throw any exception that is thrown by SimplePlostInternalAsync
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring>
			SimplePostAsync(
				winrt::hstring const& path,
				std::unordered_map<winrt::hstring, winrt::hstring> parameters
			);

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
		winrt::Windows::Foundation::Uri PrepareRequestUri(
			winrt::hstring const& path,
			std::unordered_map<winrt::hstring, winrt::hstring> parameters
		);

		/**
		 * Checks if Response is an OAuth error response.
		 * @param Response response to check
		 * @return true if Response does indicate an OAuth error, false otherwise.
		 */
		bool IsOAuthErrorResponse(
			winrt::hstring const& Response
		);

		/**
		 * Performs the actual HTTP GET request.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		/**
		 * Performs the actual HTTP DELETE request.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		/**
		 * Performs the actual HTTP POST request with no POST data.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> SimplePostInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> TryReceiveHttpResponseAsync(
			winrt::Windows::Web::Http::HttpResponseMessage const& responseMessage
		);
	};
}
