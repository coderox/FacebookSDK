#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBPermissions
	{
		FBPermissions() = default;
		FBPermissions(std::vector<std::wstring> permissions);

		std::vector<std::wstring> Values() const;
		std::wstring ToString() const;

		static winsdkfb::FBPermissions FromString(const std::wstring& permissions);
		static winsdkfb::FBPermissions Difference(const winsdkfb::FBPermissions& Minuend, const winsdkfb::FBPermissions& Subtrahend);

	private:
		static std::vector<std::wstring> ParsePermissionsFromString(
			std::wstring const& permissions
		);

		std::vector<std::wstring> _values;
	};
}