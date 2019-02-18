#pragma once

#include "FacebookPermissions.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions>
	{
		FacebookPermissions() = default;
		FacebookPermissions(Windows::Foundation::Collections::IVectorView<winrt::hstring> const& Permissions);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> Values();
		winrt::hstring ToString();

		static winsdkfb::FacebookPermissions FromString(winrt::hstring const& Permissions);
		static winsdkfb::FacebookPermissions Difference(winsdkfb::FacebookPermissions const& Minuend, winsdkfb::FacebookPermissions const& Subtrahend);

	private:
		static Windows::Foundation::Collections::IVectorView<winrt::hstring> ParsePermissionsFromString(
			winrt::hstring const& permissions
		);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> _values;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookPermissions : FacebookPermissionsT<FacebookPermissions, implementation::FacebookPermissions>
	{
	};
}
