#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBPermissions
	{
		FBPermissions() = default;
		FBPermissions(std::vector<winrt::hstring> Permissions);

		std::vector<winrt::hstring> Values() const;
		winrt::hstring ToString() const;

		static winsdkfb::FBPermissions FromString(winrt::hstring const& Permissions);
		static winsdkfb::FBPermissions Difference(const winsdkfb::FBPermissions& Minuend, const winsdkfb::FBPermissions& Subtrahend);

	private:
		static std::vector<winrt::hstring> ParsePermissionsFromString(
			winrt::hstring const& permissions
		);

		std::vector<winrt::hstring> _values;
	};
}