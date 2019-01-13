#include "pch.h"
#include "../FacebookSDK/IHttpClient.h"

namespace FacebookSDK_Tests
{
	public ref class HttpClientFake sealed : FacebookSDK::IHttpClient {
	public:
		Windows::Foundation::IAsyncOperation<Platform::String^>^ GetTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		Windows::Foundation::IAsyncOperation<Platform::String^>^ PostTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		Windows::Foundation::IAsyncOperation<Platform::String^>^ DeleteTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		Platform::String^ ParametersToQueryString(
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}
	};
}