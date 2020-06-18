#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FacebookAppRequest
	{
		FacebookAppRequest() { };

		FacebookAppRequest(
			winrt::hstring requestId,
			const winrt::Windows::Foundation::Collections::IVector<winrt::hstring> & recipients
		);

	public:
		winrt::hstring RequestId();
		winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> RecipientIds();

		static winsdkfb::FacebookAppRequest FromRequestDialogResponse(winrt::Windows::Foundation::Uri response);
	private:
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> _recipients;
		std::wstring _requestId;
	};
}
