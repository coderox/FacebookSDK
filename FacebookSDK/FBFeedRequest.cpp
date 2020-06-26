#include "FBFeedRequest.h"

#include "winrt/Windows.Foundation.Collections.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBFeedRequest::FBFeedRequest(wstring postId)
		: _postId(postId)
	{
	}

	wstring FBFeedRequest::PostId() const {
		return _postId.c_str();
	}

	winsdkfb::FBFeedRequest FBFeedRequest::FromFeedDialogResponse(Uri const& response)
	{
		winsdkfb::FBFeedRequest info;
		WwwFormUrlDecoder parameters = { nullptr };

		if (!response.Query().empty())
		{
			parameters = WwwFormUrlDecoder(response.Query());
		}

		wstring postId;

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
			info = FBFeedRequest(postId);
		}
		return info;
	}
}
