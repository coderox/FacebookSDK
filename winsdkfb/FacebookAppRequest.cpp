#include "pch.h"
#include "FacebookAppRequest.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::winsdkfb::implementation
{
	hstring FacebookAppRequest::RequestId()
	{
		return _requestId.c_str();
	}

	IVectorView<winrt::hstring> FacebookAppRequest::RecipientIds()
	{
		return _recipients.GetView();
	}

	winsdkfb::FacebookAppRequest FacebookAppRequest::FromRequestDialogResponse(Windows::Foundation::Uri const& response)
	{
		winsdkfb::FacebookAppRequest info{ nullptr };
		hstring requestId;
		auto recips{ single_threaded_vector<hstring>() };

		if (!response.Query().empty())
		{
			WwwFormUrlDecoder parameters(response.Query());

			hstring postId;

			for (unsigned int i = 0; i < parameters.Size(); i++)
			{
				IWwwFormUrlDecoderEntry entry = parameters.GetAt(i);
				wstring name(entry.Name().c_str());

				if (name == L"request")
				{
					requestId = entry.Value();
				}
				else if (name.find(L"to[") == 0)
				{
					recips.Append(entry.Value());
				}
			}

			if (!requestId.empty() && (recips.Size() > 0))
			{
				info = winrt::make<FacebookAppRequest>(requestId, recips);
			}
		}

		return info.as<winsdkfb::FacebookAppRequest>();
	}

	FacebookAppRequest::FacebookAppRequest(
		winrt::hstring requestId,
		const IVector<winrt::hstring>& recipients
	)
		: _recipients(recipients)
		, _requestId(requestId)
	{

	}
}
