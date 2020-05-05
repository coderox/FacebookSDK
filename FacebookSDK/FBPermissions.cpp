#include "FBPermissions.h"
#include <sstream>
#include "Utilities.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
{
	FBPermissions::FBPermissions(vector<hstring> permissions)
		: _values(permissions)
	{
	}

	vector<hstring> FBPermissions::Values() const
	{
		return _values;
	}

	hstring FBPermissions::ToString() const
	{
		hstring permissions;
		if (!_values.empty())
		{
			std::wstringstream wstringstream;
			for (unsigned int i = 0; i < _values.size(); i++)
			{
				if (i)
				{
					wstringstream << L",";
				}
				wstringstream << _values.at(i).c_str();
			}
			permissions = wstringstream.str().c_str();
		}

		return permissions;
	}

	FBPermissions FBPermissions::FromString(hstring const& permissions)
	{
		return FBPermissions(ParsePermissionsFromString(permissions));
	}

	FBPermissions FBPermissions::Difference(const FBPermissions& Minuend, const FBPermissions& Subtrahend)
	{
		vector<hstring> remainingPermissions;
		// stick each permissions into vector manually since copy constructor won't work with IVectorView
		for (hstring const& perm : Minuend.Values())
		{
			remainingPermissions.push_back(perm);
		}
		for (hstring const& otherPerm : Subtrahend.Values())
		{
			for (unsigned int i = 0; i < remainingPermissions.size(); ++i)
			{
				hstring perm = remainingPermissions.at(i);
				if (compare_ordinal(perm.c_str(), otherPerm.c_str()) == 0)
				{
					remainingPermissions.erase(remainingPermissions.begin() + i);
					break;
				}
			}
		}
		return FBPermissions(remainingPermissions);
	}

	vector<hstring> FBPermissions::ParsePermissionsFromString(hstring const& permissions) {
		const int bufferSize = 64;
		wstring wstringPermissions = permissions.c_str();
		wstringstream wss{ wstringPermissions };
		vector<hstring> parsedPermissions;
		wchar_t temp[bufferSize];
		while (true)
		{
			if (wss.eof())
			{
				break;
			}
			wss.getline(temp, bufferSize, L',');
			parsedPermissions.push_back(hstring(temp));
		}
		return parsedPermissions;
	}
}
