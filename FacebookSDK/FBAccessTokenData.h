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
			std::wstring const& accessToken, 
			winrt::Windows::Foundation::DateTime const& expiration,
			winrt::Windows::Foundation::DateTime const& dataAccessExpiration
		);

		FBAccessTokenData(
			std::wstring const& accessToken,
			winrt::Windows::Foundation::DateTime const& expiration,
			winrt::Windows::Foundation::DateTime const& dataAccessExpiration,
			winsdkfb::FBPermissions& grantedPermissions,
			winsdkfb::FBPermissions& declinedPermissions
		);

		std::wstring AccessToken() const;
		winrt::Windows::Foundation::DateTime ExpirationDate() const;
		winrt::Windows::Foundation::DateTime DataAccessExpirationDate() const;
		winsdkfb::FBPermissions GrantedPermissions() const;
		winsdkfb::FBPermissions DeclinedPermissions() const;
		bool IsExpired() const;
		bool IsDataAccessExpired() const;
		/**
		 * Since older tokens don't have Data Access Expiration, we need to expose that
		 * @return true if the token has a data access expiration date, false otherwise.
		 */
		bool HasDataAccessExpirationDate(
		) const {
			return _hasDataAccessExpirationDate;
		}
		void SetPermissions(std::vector<winsdkfb::Graph::FBPermission> const& perms);

		static std::any FromUri(winrt::Windows::Foundation::Uri const& response);
		
		FBAccessTokenData(
			std::wstring accessToken,
			std::wstring expiration,
			std::wstring dataAccessExpiration
		);
		/**
		 * Converts expiration string to DateTime object that indicates when
		 * the access token will expire. This value may be accessed via
		 * ExpirationDate.
		 * @param expiration The date to convert.
		 */
		void CalculateExpirationDateTime(
			std::wstring const& expiration
		);

		void CalculateDataAccessExpirationDateTime(
			std::wstring const& expiration
		);

		static winrt::Windows::Foundation::WwwFormUrlDecoder
			ParametersFromResponse(
				winrt::Windows::Foundation::Uri const& Response
			);

#ifdef _DEBUG
		void DebugPrintExpirationTime(
		) const;
#endif

		std::wstring _accessToken;
		winrt::Windows::Foundation::DateTime _expirationDate;
		winrt::Windows::Foundation::DateTime _dataAccessExpirationDate;
		winsdkfb::FBPermissions _grantedPermissions;
		winsdkfb::FBPermissions _declinedPermissions;
		bool _hasDataAccessExpirationDate = false;
	};
}
