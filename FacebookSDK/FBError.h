#pragma once

#include <winrt/Windows.Foundation.h>

namespace winsdkfb
{
	struct FBError
	{
	public:
		static FBError FromUri(winrt::Windows::Foundation::Uri const& ResponseUri);
		static FBError FromJson(winrt::hstring const& JsonText);

		FBError();
		FBError(int32_t Code, winrt::hstring const& Type, winrt::hstring const& Message);

		winrt::hstring Message();
		winrt::hstring Type();
		int32_t Code();
		int32_t Subcode();
		winrt::hstring ErrorUserTitle();
		winrt::hstring ErrorUserMessage();

	private:

		std::wstring _message;
		std::wstring _type;
		int32_t _code;
		int32_t _subcode;
		std::wstring _errorUserTitle;
		std::wstring _errorUserMessage;
	};
}
