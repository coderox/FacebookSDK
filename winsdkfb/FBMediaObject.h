#pragma once

#include "FBMediaObject.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBMediaObject : FBMediaObjectT<FBMediaObject>
	{
		FBMediaObject() = default;

		hstring ContentType();
		void ContentType(winrt::hstring const& value);
		hstring FileName();
		void FileName(winrt::hstring const& value);
		winsdkfb::FBMediaObject SetValue(winrt::array_view<const uint8_t> value);
		winrt::com_array<uint8_t> GetValue();

	private:
		std::vector<uint8_t> _value;
		std::wstring _contentType;
		std::wstring _fileName;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBMediaObject : FBMediaObjectT<FBMediaObject, implementation::FBMediaObject>
	{
	};
}
