#pragma once

#include "FacebookPermissions.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions>
	{
		FacebookPermissions() = default;
		FacebookPermissions(Windows::Foundation::Collections::IVectorView<winrt::hstring> const& Permissions);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> Values();
		winrt::hstring ToString();

		static FacebookSDK::FacebookPermissions FromString(winrt::hstring const& Permissions);
		static FacebookSDK::FacebookPermissions Difference(FacebookSDK::FacebookPermissions const& Minuend, FacebookSDK::FacebookPermissions const& Subtrahend);

	private:
		static Windows::Foundation::Collections::IVectorView<winrt::hstring> ParsePermissionsFromString(
			winrt::hstring const& permissions
		);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> _values;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions, implementation::FacebookPermissions>
	{
	};
}
