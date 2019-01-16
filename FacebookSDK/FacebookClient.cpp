#include "pch.h"
#include "FacebookClient.h"
#include "JsonClassFactory.h"
#include "SDKMessage.h"

using namespace winrt;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;

#define BufferSize (1024 * 4) // 4kb
#define AttachmentMustHavePropertiesSetError "Attachment (FBMediaObject/FBMediaStream) must have a content type, file name, and value set."
#define AttachmentValueIsNull "The value of attachment (FBMediaObject/FBMediaStream) is null."
#define UnknownResponse = "Unknown facebook response."
#define ETagKey "_etag_"
#define MultiPartFormPrefix "--"
#define MultiPartNewLine "\r\n"
#define MultiPartContentType L"Content-Type: multipart/form-data; "
#define MultiPartBoundary L"------------------------------fbsdk1234567890"
#define UserAgent L"User-Agent"
#define WinSDKFBUserAgent(version) L"FBWinSDK." version
#define WinSDKFBUserAgentString WinSDKFBUserAgent(WINSDKFB_VERSION)

namespace winrt::FacebookSDK::implementation
{
	IAsyncOperation<hstring> FacebookClient::GetTaskAsync(hstring const path, IMapView<hstring, IInspectable> const parameters)
	{
		throw hresult_not_implemented();
	}

	IAsyncOperation<hstring> FacebookClient::PostTaskAsync(hstring const path, IMapView<hstring, IInspectable> const parameters)
	{
		throw hresult_not_implemented();
	}

	IAsyncOperation<hstring> FacebookClient::DeleteTaskAsync(hstring const path, IMapView<hstring, IInspectable> const parameters)
	{
		throw hresult_not_implemented();
	}

	hstring FacebookClient::ParametersToQueryString(IMapView<hstring, IInspectable> const& parameters)
	{
		throw hresult_not_implemented();
	}

	/**
		 * Finds all FBMediaStream object in parameters.
		 * @param parameters The PropertySet to search for FBMediaStream objects in
		 * @return PropertySet containing all FBMediaStream objects found. If
		 * none are found, nullptr is instead returned.
		 */
	PropertySet FacebookClient::GetStreamsToUpload(PropertySet const& parameters) 
	{
		throw hresult_not_implemented();
	}

	/**
	 * Adds FBMediaStream objects to Form.
	 * @param Parameters PropertySet that contains FBMediaStream objects to
	 * attach to Form. Will skip any values in Parameters that cannot be
	 * cast to FBMediaStream.
	 * @param Form The form to attach FBMediaStream objects to.
	 */
	void FacebookClient::AddStreamsToForm(PropertySet const& Parameters, HttpMultipartFormDataContent const& Form) {

	}

	/**
	 * Non-stream HTTP POST flow.
	 * @param path URL to send POST request to
	 * @param parameters query parameters to attach to POST request
	 * @return IAsyncOperation containing the response content
	 * @exception Can throw any exception that is thrown by SimplePlostInternalAsync
	 */
	IAsyncOperation<hstring> FacebookClient::SimplePostAsync(hstring const& path, PropertySet const& parameters) {
		throw hresult_not_implemented();
	}

	/**
	 * HTTP POST request flow with streams.
	 * @param path URL to send POST request to
	 * @param parameters query parameters to attach to POST request
	 * @param streams streams to attach to POST request
	 * @return IASyncOperation containing the response content
	 * @exception Can throw any exception that is thrown by MultipartPostInternalAsync
	 */
	IAsyncOperation<hstring> FacebookClient::MultipartPostAsync(
		hstring const& path,
		PropertySet const& streams,
		PropertySet const& parameters) 
	{
		throw hresult_not_implemented();
	}

	/**
	 * Sorts parameters into FBMediaStream, FBMediaObject, and everything else.
	 * @param parameters The PropertySet to sort
	 * @param mediaObjects PropertySet to append FBmediaObject objects to
	 * @param mediaStreams PropertySet to append FBmediaStream objects to
	 * @return PropertySet of all objects that are not a FBMediaStrem or a
	 * FBMediaObject. If parameters is nullptr, will instead return nullptr.
	 * Note that mediaObjects and mediaStreams are both altered by this function.
	 */
	PropertySet FacebookClient::ToDictionary(
		PropertySet const& parameters,
		PropertySet const& mediaObjects,
		PropertySet const& mediaStreams
	) {
		if (parameters == nullptr)
		{
			return nullptr;
		}

		// Create a PropertySet to hold all objects that are not a MediaStream or mediaObject
		PropertySet dictionary;

		// Enumerate through all the parameters
		for (auto const& current : parameters)
		{
			hstring key(current.Key());
			if (current.Value().try_as<FacebookMediaObject>()) {
				mediaObjects.Insert(key, current.Value());
			} else if(current.Value().try_as<FacebookMediaStream>()) {
				mediaStreams.Insert(key, current.Value());
			}
			else {
				dictionary.Insert(key, current.Value());
			}
		}

		return dictionary;
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
	Uri FacebookClient::PrepareRequestUri(
		::FacebookSDK::HttpMethod httpMethod,
		hstring const& path,
		PropertySet const& parameters
	) {
		throw hresult_not_implemented();
	}

	/**
	 * Serializes non-string values in parameters to JSON strings. Note that
	 * this function modifies parameters.
	 * @param parameters The PropertySet to modify
	 */
	void FacebookClient::SerializeParameters(PropertySet const& parameters) 
	{

	}

	/**
	 * Checks if Response is an OAuth error response.
	 * @param Response response to check
	 * @return true if Response does indicate an OAuth error, false otherwise.
	 */
	bool FacebookClient::IsOAuthErrorResponse(hstring const& Response)
	{
		throw hresult_not_implemented();
	}

	/**
	 * Performs the actual HTTP GET request.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	task<hstring> FacebookClient::GetTaskInternalAsync(Uri const& RequestUri)
	{
		throw hresult_not_implemented();
	}

	/**
	 * Performs the actual HTTP DELETE request.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	task<hstring> FacebookClient::DeleteTaskInternalAsync(Uri const& RequestUri)
	{
		throw hresult_not_implemented();
	}

	/**
	 * Performs the actual HTTP POST request with no POST data.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	task<hstring> FacebookClient::SimplePostInternalAsync(Uri const& RequestUri)
	{
		throw hresult_not_implemented();
	}

	/**
	 * Performs the actual HTTP POST request with POST stream data.
	 * @param RequestUri the full URI of the request
	 * @return The response content
	 * @exception Exception Any exception that can occur during the request
	 */
	task<hstring> FacebookClient::MultipartPostInternalAsync(
		Uri const& RequestUri,
		PropertySet const& Streams
	) {
		throw hresult_not_implemented();
	}

	task<hstring> FacebookClient::TryReceiveHttpResponse(
		task<HttpResponseMessage> httpRequestTask,
		cancellation_token_source cancellationTokenSource
	) {
		throw hresult_not_implemented();
	}

	PropertySet FacebookClient::MapViewToPropertySet(IMapView<hstring, IInspectable> const& mapView) 
	{
		throw hresult_not_implemented();
	}

	void ValidateMediaStreams(PropertySet const& mediaStreams)
	{

	}

	void ValidateMediaObjects(PropertySet const& mediaObjects)
	{

	}
}
