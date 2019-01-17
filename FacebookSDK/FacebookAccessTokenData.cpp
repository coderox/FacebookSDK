#include "pch.h"
#include "FacebookAccessTokenData.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::FacebookSDK::implementation
{
	FacebookAccessTokenData::FacebookAccessTokenData(
		std::wstring accessToken,
		std::wstring expiration)
		: _accessToken(accessToken)	
	{
		if (!expiration.empty())
		{
			CalculateExpirationDateTime(expiration);
		}
#ifdef _DEBUG
		DebugPrintExpirationTime();
#endif
	}

	FacebookAccessTokenData::FacebookAccessTokenData(hstring const& accessToken, Windows::Foundation::DateTime const& expiration)
		: _accessToken(accessToken.c_str())
		, _expirationDate(expiration)
	{
#ifdef _DEBUG
		DebugPrintExpirationTime();
#endif
		std::vector<hstring> vec;
		auto v{ winrt::single_threaded_vector<hstring>() };
		//_grantedPermissions = make<FacebookPermissions>(v.GetView());
		//_declinedPermissions = make<FacebookPermissions>(v.GetView());
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

	FacebookSDK::FacebookAccessTokenData FacebookAccessTokenData::FromUri(Windows::Foundation::Uri const& response)
	{
		throw hresult_not_implemented();
	}


	/**
	 * Converts expiration string to DateTime object that indicates when
	 * the access token will expire. This value may be accessed via
	 * ExpirationDate.
	 * @param Expiration The date to convert.
	 */
	void FacebookAccessTokenData::CalculateExpirationDateTime(
		std::wstring expiration
	) {
	
	}

	WwwFormUrlDecoder FacebookAccessTokenData::ParametersFromResponse(Uri const& response) 
	{
		throw hresult_not_implemented();
	}

#ifdef _DEBUG
	void FacebookAccessTokenData::DebugPrintExpirationTime(
	) {
	
	}
#endif
}
