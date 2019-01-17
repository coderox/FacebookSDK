#pragma once

#include "FacebookPermissions.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions>
	{
		FacebookPermissions() = default;
		FacebookPermissions(Windows::Foundation::Collections::IVectorView<hstring> const& Permissions);

		Windows::Foundation::Collections::IVectorView<hstring> Values();
		hstring ToString();

		static FacebookSDK::FacebookPermissions FromString(hstring const& Permissions);
		static FacebookSDK::FacebookPermissions Difference(FacebookSDK::FacebookPermissions const& Minuend, FacebookSDK::FacebookPermissions const& Subtrahend);
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions, implementation::FacebookPermissions>
	{
	};
}
