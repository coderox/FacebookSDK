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
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> PostTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		virtual winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> DeleteTaskAsync(winrt::hstring const path, winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);
		virtual winrt::hstring ParametersToQueryString(winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const parameters);

	private:

		/**
		 * Finds all FBMediaStream object in parameters.
		 * @param parameters The PropertySet to search for FBMediaStream objects in
		 * @return PropertySet containing all FBMediaStream objects found. If
		 * none are found, nullptr is instead returned.
		 */
		winrt::Windows::Foundation::Collections::PropertySet
			GetStreamsToUpload(
				winrt::Windows::Foundation::Collections::PropertySet const& parameters
			);

		/**
		 * Adds FBMediaStream objects to Form.
		 * @param Parameters PropertySet that contains FBMediaStream objects to
		 * attach to Form. Will skip any values in Parameters that cannot be
		 * cast to FBMediaStream.
		 * @param Form The form to attach FBMediaStream objects to.
		 */
		void AddStreamsToForm(
			winrt::Windows::Foundation::Collections::PropertySet const& Parameters,
			winrt::Windows::Web::Http::HttpMultipartFormDataContent const& Form
		);

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
				winrt::Windows::Foundation::Collections::PropertySet const& parameters
			);

		/**
		 * HTTP POST request flow with streams.
		 * @param path URL to send POST request to
		 * @param parameters query parameters to attach to POST request
		 * @param streams streams to attach to POST request
		 * @return IASyncOperation containing the response content
		 * @exception Can throw any exception that is thrown by MultipartPostInternalAsync
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring>
			MultipartPostAsync(
				winrt::hstring const& path,
				winrt::Windows::Foundation::Collections::PropertySet const& streams,
				winrt::Windows::Foundation::Collections::PropertySet const& parameters
			);

		/**
		 * Sorts parameters into FBMediaStream, FBMediaObject, and everything else.
		 * @param parameters The PropertySet to sort
		 * @param mediaObjects PropertySet to append FBmediaObject objects to
		 * @param mediaStreams PropertySet to append FBmediaStream objects to
		 * @return PropertySet of all objects that are not a FBMediaStrem or a
		 * FBMediaObject. If parameters is nullptr, will instead return nullptr.
		 * Note that mediaObjects and mediaStreams are both altered by this function.
		 */
		static winrt::Windows::Foundation::Collections::PropertySet ToDictionary(
			winrt::Windows::Foundation::Collections::PropertySet const& parameters,
			winrt::Windows::Foundation::Collections::PropertySet const& mediaObjects,
			winrt::Windows::Foundation::Collections::PropertySet const& mediaStreams
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
			winrt::Windows::Foundation::Collections::PropertySet parameters
		);

		/**
		 * Serializes non-string values in parameters to JSON strings. Note that
		 * this function modifies parameters.
		 * @param parameters The PropertySet to modify
		 */
		void SerializeParameters(
			winrt::Windows::Foundation::Collections::PropertySet const& parameters
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

		/**
		 * Performs the actual HTTP POST request with POST stream data.
		 * @param RequestUri the full URI of the request
		 * @return The response content
		 * @exception Exception Any exception that can occur during the request
		 */
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> MultipartPostInternalAsync(
			winrt::Windows::Foundation::Uri const& RequestUri,
			winrt::Windows::Foundation::Collections::PropertySet const& Streams
		);

		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> TryReceiveHttpResponseAsync(
			winrt::Windows::Web::Http::HttpResponseMessage const& responseMessage
		);

		winrt::Windows::Foundation::Collections::PropertySet MapViewToPropertySet(
			winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> const mapView
		);

		void ValidateMediaStreams(
			winrt::Windows::Foundation::Collections::PropertySet const& mediaStreams
		);

		void ValidateMediaObjects(
			winrt::Windows::Foundation::Collections::PropertySet const& mediaObjects
		);
	};
}
