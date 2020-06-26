#include "FacebookAppRequest.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb
{
	wstring FacebookAppRequest::RequestId() const {
		return _requestId.c_str();
	}

	vector<wstring> FacebookAppRequest::RecipientIds() const {
		return _recipients;
	}

	FacebookAppRequest FacebookAppRequest::FromRequestDialogResponse(Uri const& response)
	{
		FacebookAppRequest info;
		wstring requestId;
		vector<wstring> recipients;

		if (!response.Query().empty())
		{
			const WwwFormUrlDecoder parameters(response.Query());

			wstring postId;

			for (unsigned int i = 0; i < parameters.Size(); i++)
			{
				auto entry = parameters.GetAt(i);
				wstring name(entry.Name().c_str());

				if (name == L"request")
				{
					requestId = entry.Value();
				}
				else if (name.find(L"to[") == 0)
				{
					recipients.push_back(entry.Value().c_str());
				}
			}

			if (!requestId.empty())
			{
				info = FacebookAppRequest(requestId, recipients);
			}
		}

		return info;
	}

	FacebookAppRequest::FacebookAppRequest(
		wstring const& requestId,
		vector<wstring> recipients)
		: _recipients(recipients)
		, _requestId(requestId)
	{

	}
}
