#pragma once

#include "FacebookMediaStream.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookMediaStream : FacebookMediaStreamT<FacebookMediaStream>
	{
		FacebookMediaStream() = delete;
		FacebookMediaStream(hstring const& fileName, Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream);

		hstring FileName();
		Windows::Storage::Streams::IRandomAccessStreamWithContentType Stream();

	private:
		Windows::Storage::Streams::IRandomAccessStreamWithContentType _stream;
		std::wstring _fileName;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookMediaStream : FacebookMediaStreamT<FacebookMediaStream, implementation::FacebookMediaStream>
	{
	};
}
