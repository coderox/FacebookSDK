#pragma once

#include "FBResult.h"
#include "Graph/FBPermission.h"
#include "FBPermissions.h"

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBAccessTokenData
	{
		FBAccessTokenData() = default;
		
		FBAccessTokenData(
			winrt::hstring const& accessToken, 
			winrt::Windows::Foundation::DateTime const& expiration,
			winrt::Windows::Foundation::DateTime const& dataAccessExpiration
		);

		FBAccessTokenData(
			winrt::hstring const& accessToken,
			winrt::Windows::Foundation::DateTime const& expiration,
			winrt::Windows::Foundation::DateTime const& dataAccessExpiration,
			winsdkfb::FBPermissions& grantedPermissions,
			winsdkfb::FBPermissions& declinedPermissions
		);

		winrt::hstring AccessToken();
		winrt::Windows::Foundation::DateTime ExpirationDate();
		winrt::Windows::Foundation::DateTime DataAccessExpirationDate();
		winsdkfb::FBPermissions GrantedPermissions();
		winsdkfb::FBPermissions DeclinedPermissions();
		bool IsExpired();
		bool IsDataAccessExpired();
		/**
		 * Since older tokens didn't have Data Access Expiration, we need to expose that
		 * @return true if the token has a data access expiration date, false otherwise.
		 */
		bool HasDataAccessExpirationDate(
		) {
			return _hasDataAccessExpirationDate;
		}
		void SetPermissions(std::vector<winsdkfb::Graph::FBPermission> const& perms);

		static std::any FromUri(winrt::Windows::Foundation::Uri const& Response);
		
		FBAccessTokenData(
			std::wstring AccessToken,
			std::wstring Expiration,
			std::wstring DataAccessExpiration
		);
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
		void DebugPrintExpirationTime(
		);
#endif

		std::wstring _accessToken;
		winrt::Windows::Foundation::DateTime _expirationDate;
		winrt::Windows::Foundation::DateTime _dataAccessExpirationDate;
		winsdkfb::FBPermissions _grantedPermissions;
		winsdkfb::FBPermissions _declinedPermissions;
		bool _hasDataAccessExpirationDate = false;
	};
}
