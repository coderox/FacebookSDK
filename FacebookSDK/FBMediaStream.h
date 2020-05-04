#pragma once

#include <winrt/Windows.Storage.Streams.h>

namespace winsdkfb
{
	struct FBMediaStream : winrt::implements<FBMediaStream, winrt::Windows::Foundation::IActivationFactory, winrt::Windows::Foundation::IInspectable>
	{
		FBMediaStream() = default;
		FBMediaStream(winrt::hstring const& fileName, winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream);

		winrt::hstring FileName();
		winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType Stream();

		winrt::hstring GetRuntimeClassName() const
		{
			return L"winsdkfb.FBMediaStream";
		}
		auto ActivateInstance() const
		{
			return winrt::make<FBMediaStream>();
		}

	private:
		winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType _stream;
		std::wstring _fileName;
	};
}
