#pragma once
#include <vector>
#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FacebookAppRequest
	{
		FacebookAppRequest() = default;

		FacebookAppRequest(
			std::wstring const& requestId,
			std::vector<std::wstring> recipients
		);

	public:
		std::wstring RequestId() const;
		std::vector<std::wstring> RecipientIds() const;

		static winsdkfb::FacebookAppRequest FromRequestDialogResponse(winrt::Windows::Foundation::Uri const& response);
	private:
		std::vector<std::wstring> _recipients;
		std::wstring _requestId;
	};
}
