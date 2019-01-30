#include "pch.h"
#include "FacebookPermissions.h"
#include <sstream>
#include "Utilities.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation::Collections;

namespace winrt::FacebookSDK::implementation
{
	FacebookPermissions::FacebookPermissions(IVectorView<hstring> const& permissions)
		: _values(permissions)
	{
	}

	IVectorView<hstring> FacebookPermissions::Values()
	{
		return _values;
	}

	hstring FacebookPermissions::ToString()
	{
		hstring permissions;
		if (_values)
		{
			std::wstringstream wstringstream;
			for (unsigned int i = 0; i < _values.Size(); i++)
			{
				if (i)
				{
					wstringstream << L",";
				}
				wstringstream << _values.GetAt(i).c_str();
			}
			permissions = wstringstream.str().c_str();
		}

		return permissions;
	}

	FacebookSDK::FacebookPermissions FacebookPermissions::FromString(hstring const& permissions)
	{
		return make<FacebookPermissions>(ParsePermissionsFromString(permissions));
	}

	FacebookSDK::FacebookPermissions FacebookPermissions::Difference(FacebookSDK::FacebookPermissions const& Minuend, FacebookSDK::FacebookPermissions const& Subtrahend)
	{
		IVector<hstring> remainingPermissions = { single_threaded_vector<hstring>() };
		// stick each permissions into vector manually since copy constructor won't work with IVectorView
		for (hstring const& perm : Minuend.Values())
		{
			remainingPermissions.Append(perm);
		}
		for (hstring const& otherPerm : Subtrahend.Values())
		{
			for (unsigned int i = 0; i < remainingPermissions.Size(); ++i)
			{
				hstring perm = remainingPermissions.GetAt(i);
				if (compare_ordinal(perm.c_str(), otherPerm.c_str()) == 0)
				{
					remainingPermissions.RemoveAt(i);
					break;
				}
			}
		}
		return make<FacebookPermissions>(remainingPermissions.GetView());
	}

	IVectorView<hstring> FacebookPermissions::ParsePermissionsFromString(hstring const& permissions) {
		const int bufferSize = 64;
		wstring wstringPermissions = permissions.c_str();
		wstringstream wss{ wstringPermissions };
		IVector<hstring> parsedPermissions = { single_threaded_vector<hstring>() };
		wchar_t temp[bufferSize];
		while (true)
		{
			if (wss.eof())
			{
				break;
			}
			wss.getline(temp, bufferSize, L',');
			parsedPermissions.Append(hstring(temp));
		}
		return parsedPermissions.GetView();
	}
}
