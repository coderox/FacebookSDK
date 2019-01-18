#pragma once

#include "FacebookFeedRequest.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookFeedRequest : FacebookFeedRequestT<FacebookFeedRequest>
	{
		FacebookFeedRequest() = default;
		FacebookFeedRequest(winrt::hstring postId);

		hstring PostId();

		static FacebookSDK::FacebookFeedRequest FromFeedDialogResponse(Windows::Foundation::Uri const& response);

	private:
		std::wstring _postId;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookFeedRequest : FacebookFeedRequestT<FacebookFeedRequest, implementation::FacebookFeedRequest>
	{
	};
}
