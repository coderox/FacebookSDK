#include "pch.h"

namespace FacebookSDK_Tests
{
	public ref class HttpClientFake sealed : FacebookSDK::IHttpClient {
	public:
		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ GetTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ PostTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ DeleteTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}

		virtual Platform::String^ ParametersToQueryString(
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		) {
			throw ref new Platform::NotImplementedException();
		}
	};
}