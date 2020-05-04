#include "FBMediaStream.h"

using namespace winrt;
using namespace Windows::Storage::Streams;

namespace winsdkfb
{
	FBMediaStream::FBMediaStream(hstring const& fileName, IRandomAccessStreamWithContentType const& stream)
	{
		_fileName = fileName.c_str();
		_stream = stream;
	}

	hstring FBMediaStream::FileName()
	{
		return _fileName.c_str();
	}

	IRandomAccessStreamWithContentType FBMediaStream::Stream()
	{
		return _stream;
	}
}
