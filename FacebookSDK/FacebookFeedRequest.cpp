#include "pch.h"
#include "FacebookFeedRequest.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::FacebookSDK::implementation
{
	FacebookFeedRequest::FacebookFeedRequest(hstring postId)
		: _postId(postId.c_str())
	{
	}

	hstring FacebookFeedRequest::PostId()
	{
		return _postId.c_str();
	}

	FacebookSDK::FacebookFeedRequest FacebookFeedRequest::FromFeedDialogResponse(Windows::Foundation::Uri const& response)
	{
		FacebookSDK::FacebookFeedRequest info = { nullptr };
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
			info = make<FacebookFeedRequest>(postId);
		}
		return info;
	}
}
