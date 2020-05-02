#pragma once

#include <winrt/base.h>

namespace winsdkfb
{
	struct FBMediaObject
	{
		FBMediaObject() = default;

		winrt::hstring ContentType();
		void ContentType(winrt::hstring const& value);
		winrt::hstring FileName();
		void FileName(winrt::hstring const& value);
		winsdkfb::FBMediaObject SetValue(winrt::array_view<const uint8_t> value);
		winrt::com_array<uint8_t> GetValue();

	private:
		std::vector<uint8_t> _value;
		std::wstring _contentType;
		std::wstring _fileName;
	};
}
