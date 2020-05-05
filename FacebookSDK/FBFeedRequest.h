#pragma once

#include <winrt/Windows.Foundation.h>

namespace winsdkfb
{
	struct FBFeedRequest
	{
		FBFeedRequest() = default;
		FBFeedRequest(winrt::hstring postId);

		winrt::hstring PostId();

		static winsdkfb::FBFeedRequest FromFeedDialogResponse(winrt::Windows::Foundation::Uri const& response);

	private:
		std::wstring _postId;
	};
}
