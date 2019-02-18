#pragma once

#include "FacebookFeedRequest.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookFeedRequest : FacebookFeedRequestT<FacebookFeedRequest>
	{
		FacebookFeedRequest() = default;
		FacebookFeedRequest(winrt::hstring postId);

		hstring PostId();

		static winsdkfb::FacebookFeedRequest FromFeedDialogResponse(Windows::Foundation::Uri const& response);

	private:
		std::wstring _postId;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookFeedRequest : FacebookFeedRequestT<FacebookFeedRequest, implementation::FacebookFeedRequest>
	{
	};
}
