#include "pch.h"
#include "FacebookMediaObject.h"

using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winrt::FacebookSDK::implementation
{
	hstring FacebookMediaObject::ContentType()
	{
		return _contentType.c_str();
	}

	void FacebookMediaObject::ContentType(hstring const& value)
	{
		_contentType = value.c_str();
	}

	hstring FacebookMediaObject::FileName()
	{
		return _fileName.c_str();
	}

	void FacebookMediaObject::FileName(hstring const& value)
	{
		_fileName = value.c_str();
	}

	FacebookSDK::FacebookMediaObject FacebookMediaObject::SetValue(array_view<const uint8_t> value)
	{
		_value.clear();
		for (auto item : value)
		{
			_value.push_back(item);
		}
		return *this;
	}

	com_array<uint8_t> FacebookMediaObject::GetValue()
	{
		winrt::com_array<uint8_t> result(_value);
		return result;
	}
}
