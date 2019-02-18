#pragma once

#include "FBError.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBError : FBErrorT<FBError>
	{
	public:
		static winsdkfb::FBError FromUri(Windows::Foundation::Uri const& ResponseUri);
		static winsdkfb::FBError FromJson(hstring const& JsonText);

		FBError();
		FBError(int32_t Code, hstring const& Type, hstring const& Message);

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

namespace winrt::winsdkfb::factory_implementation
{
	struct FBError : FBErrorT<FBError, implementation::FBError>
	{
	};
}
