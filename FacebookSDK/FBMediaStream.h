#pragma once

#include <winrt/Windows.Storage.Streams.h>

namespace winsdkfb
{
	struct FBMediaStream 
	{
		FBMediaStream() = delete;
		FBMediaStream(winrt::hstring const& fileName, winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream);

		winrt::hstring FileName();
		winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType Stream();

	private:
		winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType _stream;
		std::wstring _fileName;
	};
}
