#include "FBPermissions.h"
#include <sstream>
#include "Utilities.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
{
	FBPermissions::FBPermissions(std::vector<wstring> permissions)
		: _values(permissions)
	{
	}

	std::vector<wstring> FBPermissions::Values() const
	{
		return _values;
	}

	wstring FBPermissions::ToString() const
	{
		wstring permissions;
		if (!_values.empty())
		{
			wstringstream permissionsStream;
			for (unsigned int i = 0; i < _values.size(); i++)
			{
				if (i)
				{
					permissionsStream << L",";
				}
				permissionsStream << _values[i].c_str();
			}
			permissions = permissionsStream.str().c_str();
		}

		return permissions;
	}

	FBPermissions FBPermissions::FromString(wstring const& permissions)
	{
		const auto parsedPermissions = ParsePermissionsFromString(permissions);
		return FBPermissions(parsedPermissions);
	}

	FBPermissions FBPermissions::Difference(const FBPermissions& Minuend, const FBPermissions& Subtrahend)
	{
		std::vector<wstring> remainingPermissions;
		// stick each permissions into vector manually since copy constructor won't work with IVectorView
		for (wstring const& perm : Minuend.Values())
		{
			remainingPermissions.push_back(perm);
		}
		for (wstring const& otherPerm : Subtrahend.Values())
		{
			for (unsigned int i = 0; i < remainingPermissions.size(); ++i)
			{
				wstring perm = remainingPermissions.at(i);
				if (compare_ordinal(perm.c_str(), otherPerm.c_str()) == 0)
				{
					remainingPermissions.erase(remainingPermissions.begin() + i);
					break;
				}
			}
		}
		return FBPermissions(remainingPermissions);
	}

	std::vector<wstring> FBPermissions::ParsePermissionsFromString(const std::wstring& permissions) {
		const auto bufferSize = 64;
		const std::wstring wPermissions = permissions.c_str();
		std::wstringstream wss{ wPermissions };
		std::vector<wstring> parsedPermissions;
		wchar_t temp[bufferSize];
		while (true)
		{
			if (wss.eof())
			{
				break;
			}
			wss.getline(temp, bufferSize, L',');
			parsedPermissions.emplace_back(wstring(temp));
		}
		return parsedPermissions;
	}
}
