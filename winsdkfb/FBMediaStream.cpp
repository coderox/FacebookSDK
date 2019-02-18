#include "pch.h"
#include "FBMediaStream.h"

namespace winrt::winsdkfb::implementation
{
	FBMediaStream::FBMediaStream(hstring const& fileName, Windows::Storage::Streams::IRandomAccessStreamWithContentType const& stream)
	{
		_fileName = fileName.c_str();
		_stream = stream;
	}

	hstring FBMediaStream::FileName()
	{
		return _fileName.c_str();
	}

	Windows::Storage::Streams::IRandomAccessStreamWithContentType FBMediaStream::Stream()
	{
		return _stream;
	}
}
