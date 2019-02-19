#include "pch.h"
#include "FBPermissions.h"
#include <sstream>
#include "Utilities.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation::Collections;

namespace winrt::winsdkfb::implementation
{
	FBPermissions::FBPermissions(IVectorView<hstring> const& permissions)
		: _values(permissions)
	{
	}

	IVectorView<hstring> FBPermissions::Values()
	{
		return _values;
	}

	hstring FBPermissions::ToString()
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

	winsdkfb::FBPermissions FBPermissions::FromString(hstring const& permissions)
	{
		return make<FBPermissions>(ParsePermissionsFromString(permissions));
	}

	winsdkfb::FBPermissions FBPermissions::Difference(winsdkfb::FBPermissions const& Minuend, winsdkfb::FBPermissions const& Subtrahend)
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
		return make<FBPermissions>(remainingPermissions.GetView());
	}

	IVectorView<hstring> FBPermissions::ParsePermissionsFromString(hstring const& permissions) {
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
