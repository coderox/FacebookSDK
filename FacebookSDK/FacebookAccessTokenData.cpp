#include "pch.h"
#include "FacebookAccessTokenData.h"

namespace winrt::FacebookSDK::implementation
{
	FacebookAccessTokenData::FacebookAccessTokenData(hstring const& AccessToken, Windows::Foundation::DateTime const& Expiration)
	{
		throw hresult_not_implemented();
	}

	hstring FacebookAccessTokenData::AccessToken()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::DateTime FacebookAccessTokenData::ExpirationDate()
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookPermissions FacebookAccessTokenData::GrantedPermissions()
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookPermissions FacebookAccessTokenData::DeclinedPermissions()
	{
		throw hresult_not_implemented();
	}

	bool FacebookAccessTokenData::IsExpired()
	{
		throw hresult_not_implemented();
	}

	void FacebookAccessTokenData::SetPermissions(Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> const& perms)
	{
		throw hresult_not_implemented();
	}

	FacebookSDK::FacebookAccessTokenData FacebookAccessTokenData::FromUri(Windows::Foundation::Uri const& Response)
	{
		throw hresult_not_implemented();
	}
}
