#include "pch.h"
#include "FBMediaObject.h"

using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winrt::winsdkfb::implementation
{
	hstring FBMediaObject::ContentType()
	{
		return _contentType.c_str();
	}

	void FBMediaObject::ContentType(hstring const& value)
	{
		_contentType = value.c_str();
	}

	hstring FBMediaObject::FileName()
	{
		return _fileName.c_str();
	}

	void FBMediaObject::FileName(hstring const& value)
	{
		_fileName = value.c_str();
	}

	winsdkfb::FBMediaObject FBMediaObject::SetValue(array_view<const uint8_t> value)
	{
		_value.clear();
		for (auto item : value)
		{
			_value.push_back(item);
		}
		return *this;
	}

	com_array<uint8_t> FBMediaObject::GetValue()
	{
		winrt::com_array<uint8_t> result(_value);
		return result;
	}
}
