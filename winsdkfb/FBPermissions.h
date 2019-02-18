#pragma once

#include "FBPermissions.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBPermissions : FBPermissionsT<FBPermissions>
	{
		FBPermissions() = default;
		FBPermissions(Windows::Foundation::Collections::IVectorView<winrt::hstring> const& Permissions);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> Values();
		winrt::hstring ToString();

		static winsdkfb::FBPermissions FromString(winrt::hstring const& Permissions);
		static winsdkfb::FBPermissions Difference(winsdkfb::FBPermissions const& Minuend, winsdkfb::FBPermissions const& Subtrahend);

	private:
		static Windows::Foundation::Collections::IVectorView<winrt::hstring> ParsePermissionsFromString(
			winrt::hstring const& permissions
		);

		Windows::Foundation::Collections::IVectorView<winrt::hstring> _values;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBPermissions : FBPermissionsT<FBPermissions, implementation::FBPermissions>
	{
	};
}
