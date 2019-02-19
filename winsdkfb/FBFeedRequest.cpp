#include "pch.h"
#include "FBFeedRequest.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::winsdkfb::implementation
{
	FBFeedRequest::FBFeedRequest(hstring postId)
		: _postId(postId.c_str())
	{
	}

	hstring FBFeedRequest::PostId()
	{
		return _postId.c_str();
	}

	winsdkfb::FBFeedRequest FBFeedRequest::FromFeedDialogResponse(Windows::Foundation::Uri const& response)
	{
		winsdkfb::FBFeedRequest info = { nullptr };
		WwwFormUrlDecoder parameters = { nullptr };

		if (!response.Query().empty())
		{
			parameters = WwwFormUrlDecoder(response.Query());
		}

		hstring postId;

		for (unsigned int i = 0; parameters && i < parameters.Size(); i++)
		{
			auto entry = parameters.GetAt(i);

			if (entry.Name() == L"post_id")
			{
				postId = entry.Value();
			}
		}

		if (!postId.empty())
		{
			info = make<FBFeedRequest>(postId);
		}
		return info;
	}
}
