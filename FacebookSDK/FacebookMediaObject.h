#pragma once

#include "FacebookMediaObject.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookMediaObject : FacebookMediaObjectT<FacebookMediaObject>
	{
		FacebookMediaObject() = delete;

		hstring ContentType();
		void ContentType(hstring const& value);
		hstring FileName();
		void FileName(hstring const& value);
		FacebookSDK::FacebookMediaObject SetValue(array_view<uint8_t const> value);
		com_array<uint8_t> GetValue();
	};
}
