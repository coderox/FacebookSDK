#pragma once


namespace winsdkfb_Tests
{
	public ref class MockHttpClient sealed : winsdkfb::IHttpClient {
	public:

		property Platform::String^ ResponseData {
			Platform::String^ get() { return responseData; }
			void set(Platform::String^ value) { responseData = value; }
		}

		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ GetTaskAsync(Platform::String^ path, Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters);

		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ PostTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		);

		virtual Windows::Foundation::IAsyncOperation<Platform::String^>^ DeleteTaskAsync(
			Platform::String^ path,
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		);

		virtual Platform::String^ ParametersToQueryString(
			Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ parameters
		);

	private:
		Platform::String^ responseData;
	};
}