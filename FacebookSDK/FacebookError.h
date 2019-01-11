#pragma once

#include "FacebookError.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookError : FacebookErrorT<FacebookError>
	{
	public:
		static FacebookSDK::FacebookError FromUri(Windows::Foundation::Uri const& ResponseUri);
		static FacebookSDK::FacebookError FromJson(hstring const& JsonText);

		FacebookError();
		FacebookError(int32_t Code, hstring const& Type, hstring const& Message);

		hstring Message();
		hstring Type();
		int32_t Code();
		int32_t Subcode();
		hstring ErrorUserTitle();
		hstring ErrorUserMessage();

	private:

		std::wstring _message;
		std::wstring _type;
		int32_t _code;
		int32_t _subcode;
		std::wstring _errorUserTitle;
		std::wstring _errorUserMessage;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookError : FacebookErrorT<FacebookError, implementation::FacebookError>
	{
	};
}
