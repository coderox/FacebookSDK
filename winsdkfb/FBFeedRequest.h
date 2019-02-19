#pragma once

#include "FBFeedRequest.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBFeedRequest : FBFeedRequestT<FBFeedRequest>
	{
		FBFeedRequest() = default;
		FBFeedRequest(winrt::hstring postId);

		hstring PostId();

		static winsdkfb::FBFeedRequest FromFeedDialogResponse(Windows::Foundation::Uri const& response);

	private:
		std::wstring _postId;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBFeedRequest : FBFeedRequestT<FBFeedRequest, implementation::FBFeedRequest>
	{
	};
}
