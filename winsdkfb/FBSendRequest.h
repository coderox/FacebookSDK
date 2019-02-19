#pragma once

#include "FBSendRequest.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBSendRequest : FBSendRequestT<FBSendRequest>
	{
		FBSendRequest() = default;

	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBSendRequest : FBSendRequestT<FBSendRequest, implementation::FBSendRequest>
	{
	};
}
