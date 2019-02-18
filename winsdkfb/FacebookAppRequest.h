#pragma once

#include "FacebookAppRequest.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookAppRequest : FacebookAppRequestT<FacebookAppRequest>
	{
		FacebookAppRequest() { };

		FacebookAppRequest(
			winrt::hstring requestId,
			const winrt::Windows::Foundation::Collections::IVector<winrt::hstring> & recipients
		);

	public:
		winrt::hstring RequestId();
		winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> RecipientIds();

		static winsdkfb::FacebookAppRequest FromRequestDialogResponse(Windows::Foundation::Uri const& response);
	private:
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> _recipients;
		std::wstring _requestId;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookAppRequest : FacebookAppRequestT<FacebookAppRequest, implementation::FacebookAppRequest>
	{
	};
}
