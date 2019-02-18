#pragma once

#include "FBAccessTokenData.g.h"
#include "FBPermissions.h"

namespace winrt::winsdkfb::implementation
{
	struct FBAccessTokenData : FBAccessTokenDataT<FBAccessTokenData>
	{
		FBAccessTokenData() = default;
		FBAccessTokenData(hstring const& AccessToken, 
			Windows::Foundation::DateTime const& expiration
		);

		FBAccessTokenData(hstring const& AccessToken, 
			Windows::Foundation::DateTime const& expiration,
			Windows::Foundation::DateTime const& dataAccessExpiration
		);

		hstring AccessToken();
		Windows::Foundation::DateTime ExpirationDate();
		Windows::Foundation::DateTime DataAccessExpirationDate();
		winsdkfb::FBPermissions GrantedPermissions();
		winsdkfb::FBPermissions DeclinedPermissions();
		bool IsExpired();
		bool HasExpirationDate();

		bool IsDataAccessExpired();
		bool HasDataAccessExpirationDate();
		
		void SetPermissions(Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> const& perms);

		static winsdkfb::FBAccessTokenData FromUri(Windows::Foundation::Uri const& Response);

		FBAccessTokenData(
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
		bool _hasExpirationDate;
		bool _hasDataAccessExpirationDate;
		winsdkfb::FBPermissions _grantedPermissions{ nullptr };
		winsdkfb::FBPermissions _declinedPermissions{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBAccessTokenData : FBAccessTokenDataT<FBAccessTokenData, implementation::FBAccessTokenData>
	{
	};
}
