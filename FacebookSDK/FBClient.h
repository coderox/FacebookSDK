#pragma once

#include "IHttpClient.h"

#include <winrt/Windows.Web.Http.h>

namespace winsdkfb
{
	struct FBClient final : public IHttpClient
	{
		FBClient() = default;

		// IHttpClient
		concurrency::task<std::wstring> GetTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) override;
		concurrency::task<std::wstring> PostTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) override;
		concurrency::task<std::wstring> DeleteTaskAsync(std::wstring const path, std::unordered_map<std::wstring, std::wstring> const parameters) override;
		std::wstring ParametersToQueryString(std::unordered_map<std::wstring, std::wstring> const parameters) override;

	private:

		/**
		 * Non-stream HTTP POST flow.
		 * @param path URL to send POST request to
		 * @param parameters query parameters to attach to POST request
		 * @return IAsyncOperation containing the response content
		 * @exception Can throw any exception that is thrown by SimplePostInternalAsync
		 */
		static concurrency::task<std::wstring>
			SimplePostAsync(
				std::wstring const& path,
				std::unordered_map<std::wstring, std::wstring> parameters
			);

		/**
		 * Builds request URI.
		 * @param path Request path
		 * @param parameters Query parameters for the request
		 * @return Request URI
		 * @exception FailureExecption if FBMediaObject or FBMediaStream are
		 * attempting to be attached on non-POST requests.
		 * @exception InvalidArgumentException if httpMethod is POST and improperly formatted/empty media object is attached.
		 */
		static winrt::Windows::Foundation::Uri PrepareRequestUri(
			std::wstring const& path,
			const std::unordered_map<std::wstring, std::wstring>& parameters
		);

		/**
		 * Checks if Response is an OAuth error response.
		 * @param Response response to check
		 * @return true if Response does indicate an OAuth error, false otherwise.
		 */
		static bool IsOAuthErrorResponse(
			std::wstring const& Response
		);

		/**
		 * Performs the actual HTTP GET request.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		static concurrency::task<std::wstring> GetTaskInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		/**
		 * Performs the actual HTTP DELETE request.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		static concurrency::task<std::wstring> DeleteTaskInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		/**
		 * Performs the actual HTTP POST request with no POST data.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		static concurrency::task<std::wstring> SimplePostInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri
		);

		static concurrency::task<std::wstring> TryReceiveHttpResponseAsync(
			winrt::Windows::Web::Http::HttpResponseMessage const& responseMessage
		);
	};
}
