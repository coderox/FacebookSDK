#pragma once

#include "FacebookSendRequest.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookSendRequest : FacebookSendRequestT<FacebookSendRequest>
	{
		FacebookSendRequest() = default;

	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookSendRequest : FacebookSendRequestT<FacebookSendRequest, implementation::FacebookSendRequest>
	{
	};
}
