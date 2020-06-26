#pragma once

#include <winrt/Windows.Foundation.h>

namespace winsdkfb
{
	struct FBFeedRequest
	{
		FBFeedRequest() = default;
		FBFeedRequest(std::wstring postId);

		std::wstring PostId() const;

		static winsdkfb::FBFeedRequest FromFeedDialogResponse(winrt::Windows::Foundation::Uri const& response);

	private:
		std::wstring _postId;
	};
}
