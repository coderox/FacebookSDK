#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBPermissions
	{
		FBPermissions() = default;
		FBPermissions(std::vector<winrt::hstring> Permissions);

		std::vector<winrt::hstring> Values();
		winrt::hstring ToString();

		static std::shared_ptr<winsdkfb::FBPermissions> FromString(winrt::hstring const& Permissions);
		static std::shared_ptr<winsdkfb::FBPermissions> Difference(std::shared_ptr<winsdkfb::FBPermissions> Minuend, std::shared_ptr<winsdkfb::FBPermissions> Subtrahend);

	private:
		static std::vector<winrt::hstring> ParsePermissionsFromString(
			winrt::hstring const& permissions
		);

		std::vector<winrt::hstring> _values;
	};
}