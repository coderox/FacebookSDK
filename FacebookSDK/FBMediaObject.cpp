#include "FBMediaObject.h"

#include <winrt/Windows.Foundation.Collections.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
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

	void FBMediaObject::SetValue(vector<uint8_t> value)
	{
		_value.clear();
		for (auto item : value)
		{
			_value.push_back(item);
		}
	}

	std::vector<uint8_t> FBMediaObject::GetValue()
	{
		std::vector<uint8_t> result(_value);
		return result;
	}
}
