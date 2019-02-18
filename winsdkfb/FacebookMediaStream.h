#pragma once

#include "FacebookMediaStream.g.h"

namespace winrt::winsdkfb::implementation
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

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookMediaStream : FacebookMediaStreamT<FacebookMediaStream, implementation::FacebookMediaStream>
	{
	};
}
