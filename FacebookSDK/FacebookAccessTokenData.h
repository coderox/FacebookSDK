#pragma once

#include "FacebookAccessTokenData.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookAccessTokenData : FacebookAccessTokenDataT<FacebookAccessTokenData>
	{
		FacebookAccessTokenData() = delete;
		FacebookAccessTokenData(hstring const& AccessToken, Windows::Foundation::DateTime const& Expiration);

		hstring AccessToken();
		Windows::Foundation::DateTime ExpirationDate();
		FacebookSDK::FacebookPermissions GrantedPermissions();
		FacebookSDK::FacebookPermissions DeclinedPermissions();
		bool IsExpired();
		void SetPermissions(Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> const& perms);

		static FacebookSDK::FacebookAccessTokenData FromUri(Windows::Foundation::Uri const& Response);
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookAccessTokenData : FacebookAccessTokenDataT<FacebookAccessTokenData, implementation::FacebookAccessTokenData>
	{
	};
}
