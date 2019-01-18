#include "pch.h"
#include "MockHttpClient.h"
#include <ppltasks.h>

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace FacebookSDK_Tests
{

	IAsyncOperation<String^>^ MockHttpClient::GetTaskAsync(String^ path, IMapView<String^, Object^>^ parameters)
	{
		return create_async([=]() {
			return ResponseData;
		});
	}

	Windows::Foundation::IAsyncOperation<Platform::String^>^ MockHttpClient::PostTaskAsync(
		Platform::String^ path,
		Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
	) {
		throw ref new Platform::NotImplementedException();
	}

	Windows::Foundation::IAsyncOperation<Platform::String^>^ MockHttpClient::DeleteTaskAsync(
		Platform::String^ path,
		Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
	) {
		throw ref new Platform::NotImplementedException();
	}

	Platform::String^ MockHttpClient::ParametersToQueryString(
		Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
	) {
		throw ref new Platform::NotImplementedException();
	}
}
