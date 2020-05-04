#pragma once

#include "FBResult.h"
#include "Graph/FBPermission.h"
#include "FBPermissions.h"

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBAccessTokenData : public FBResult
	{
		FBAccessTokenData() = default;
		FBAccessTokenData(winrt::hstring const& AccessToken, winrt::Windows::Foundation::DateTime const& Expiration);

		winrt::hstring AccessToken();
		winrt::Windows::Foundation::DateTime ExpirationDate();
		winsdkfb::FBPermissions GrantedPermissions();
		winsdkfb::FBPermissions DeclinedPermissions();
		bool IsExpired();
		void SetPermissions(std::vector<winsdkfb::Graph::FBPermission> const& perms);

		static winsdkfb::FBAccessTokenData FromUri(winrt::Windows::Foundation::Uri const& Response);

		FBAccessTokenData(
			std::wstring accessToken,
			std::wstring expiration
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
		winsdkfb::FBPermissions _grantedPermissions;
		winsdkfb::FBPermissions _declinedPermissions;
	};
}
