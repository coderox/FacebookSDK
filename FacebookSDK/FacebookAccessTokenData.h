#pragma once

#include "FacebookAccessTokenData.g.h"
#include "FacebookPermissions.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookAccessTokenData : FacebookAccessTokenDataT<FacebookAccessTokenData>
	{
		FacebookAccessTokenData() = default;
		FacebookAccessTokenData(hstring const& accessToken,
			Windows::Foundation::DateTime const& expiration
		);

		FacebookAccessTokenData(hstring const& accessToken,
			Windows::Foundation::DateTime const& expiration,
			Windows::Foundation::DateTime const& dataAccessExpiration
		);

		hstring AccessToken();
		Windows::Foundation::DateTime ExpirationDate();
		Windows::Foundation::DateTime DataAccessExpirationDate();
		FacebookSDK::FacebookPermissions GrantedPermissions();
		FacebookSDK::FacebookPermissions DeclinedPermissions();
		bool IsExpired();
		bool IsDataAccessExpired();
		bool HasDataAccessExpired();
		void SetPermissions(Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> const& perms);

		static FacebookSDK::FacebookAccessTokenData FromUri(Windows::Foundation::Uri const& Response);

		FacebookAccessTokenData(
			std::wstring accessToken,
			std::wstring expiration,
			std::wstring dataAccessExpiration
		);

	private:

		/**
		 * Converts expiration string to DateTime object that indicates when
		 * the access token will expire. This value may be accessed via
		 * ExpirationDate.
		 * @param Expiration The date to convert.
		 */
		void CalculateExpirationDateTime(
			std::wstring expiration
		);

		void CalculateDataAccessExpirationDateTime(
			std::wstring expiration
		);

		static winrt::Windows::Foundation::WwwFormUrlDecoder
			ParametersFromResponse(
				winrt::Windows::Foundation::Uri const& Response
			);

#ifdef _DEBUG
		void DebugPrintExpirationTime();
#endif

		std::wstring _accessToken;
		winrt::Windows::Foundation::DateTime _expirationDate;
		winrt::Windows::Foundation::DateTime _dataAccessExpirationDate;
		bool _hasDataAccessExpirationDate;
		FacebookSDK::FacebookPermissions _grantedPermissions{ nullptr };
		FacebookSDK::FacebookPermissions _declinedPermissions{ nullptr };
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookAccessTokenData : FacebookAccessTokenDataT<FacebookAccessTokenData, implementation::FacebookAccessTokenData>
	{
	};
}
