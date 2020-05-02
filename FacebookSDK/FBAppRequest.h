#pragma once

#include <winrt/Windows.Foundation.Collections.h>

namespace winsdkfb
{
	struct FBAppRequest
	{
		FBAppRequest() { };

		FBAppRequest(
			winrt::hstring requestId,
			const winrt::Windows::Foundation::Collections::IVector<winrt::hstring> & recipients
		);

	public:
		winrt::hstring RequestId();
		winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> RecipientIds();

		static winsdkfb::FBAppRequest FromRequestDialogResponse(Windows::Foundation::Uri const& response);
	private:
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> _recipients;
		std::wstring _requestId;
	};
}
