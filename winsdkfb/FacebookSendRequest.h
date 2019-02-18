#pragma once

#include "FacebookSendRequest.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookSendRequest : FacebookSendRequestT<FacebookSendRequest>
	{
		FacebookSendRequest() = default;

	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookSendRequest : FacebookSendRequestT<FacebookSendRequest, implementation::FacebookSendRequest>
	{
	};
}
