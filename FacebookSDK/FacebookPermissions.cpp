#include "pch.h"
#include "FacebookPermissions.h"

namespace winrt::FacebookSDK::implementation
{
	FacebookPermissions::FacebookPermissions(Windows::Foundation::Collections::IVectorView<hstring> const& Permissions)
	{
		
	}

	Windows::Foundation::Collections::IVectorView<hstring> FacebookPermissions::Values()
	{
		throw hresult_not_implemented();
	}

	hstring FacebookPermissions::ToString()
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookPermissions FacebookPermissions::FromString(hstring const& Permissions)
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookPermissions FacebookPermissions::Difference(FacebookSDK::FacebookPermissions const& Minuend, FacebookSDK::FacebookPermissions const& Subtrahend)
	{
		throw hresult_not_implemented();
	}
}
