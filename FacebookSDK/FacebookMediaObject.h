#pragma once

#include "FacebookMediaObject.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookMediaObject : FacebookMediaObjectT<FacebookMediaObject>
	{
		FacebookMediaObject() = default;

		hstring ContentType();
		void ContentType(winrt::hstring const& value);
		hstring FileName();
		void FileName(winrt::hstring const& value);
		FacebookSDK::FacebookMediaObject SetValue(winrt::array_view<const uint8_t> value);
		winrt::com_array<uint8_t> GetValue();

	private:
		std::vector<uint8_t> _value;
		std::wstring _contentType;
		std::wstring _fileName;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookMediaObject : FacebookMediaObjectT<FacebookMediaObject, implementation::FacebookMediaObject>
	{
	};
}
