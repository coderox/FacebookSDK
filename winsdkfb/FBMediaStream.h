#pragma once

#include "FBMediaStream.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBMediaStream : FBMediaStreamT<FBMediaStream>
	{
		FBMediaStream() = delete;
		FBMediaStream(hstring const& fileName, Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream);

		hstring FileName();
		Windows::Storage::Streams::IRandomAccessStreamWithContentType Stream();

	private:
		Windows::Storage::Streams::IRandomAccessStreamWithContentType _stream;
		std::wstring _fileName;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBMediaStream : FBMediaStreamT<FBMediaStream, implementation::FBMediaStream>
	{
	};
}
