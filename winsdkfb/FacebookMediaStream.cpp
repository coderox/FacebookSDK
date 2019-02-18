#include "pch.h"
#include "FacebookMediaStream.h"

namespace winrt::winsdkfb::implementation
{
	FacebookMediaStream::FacebookMediaStream(hstring const& fileName, Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream)
	{
		_fileName = fileName.c_str();
		_stream = stream;
	}

	hstring FacebookMediaStream::FileName()
	{
		return _fileName.c_str();
	}

	Windows::Storage::Streams::IRandomAccessStreamWithContentType FacebookMediaStream::Stream()
	{
		return _stream;
	}
}
