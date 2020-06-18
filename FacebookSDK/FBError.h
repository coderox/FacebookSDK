#pragma once

#include <winrt/Windows.Foundation.h>

namespace winsdkfb
{
	enum class ErrorCode {
		ErrorCodeOauthException = 190,
		ErrorCodeWebAccountProviderNotFound = 5000,
		ErrorCodeWebTokenRequestStatus = 5001,
		ErrorCodeWebAccountProviderNotSupported = 5002,
	};

	enum class ErrorSubCode {
		ErrorSubcodeAppNotAuthorized = 458,
		ErrorSubcodeSessionInvalidated = 466
	};

	struct FBError
	{
	public:
		static FBError FromUri(winrt::Windows::Foundation::Uri const& responseUri);
		static FBError FromJson(winrt::hstring const& jsonText);

		FBError();
		FBError(int32_t code, std::wstring type, std::wstring message);
		FBError(std::nullptr_t) noexcept {}

		std::wstring Message();
		std::wstring Type();
		int32_t Code();
		int32_t Subcode();
		std::wstring ErrorUserTitle();
		std::wstring ErrorUserMessage();

	private:

		std::wstring _message;
		std::wstring _type;
		int32_t _code = 0;
		int32_t _subcode = 0;
		std::wstring _errorUserTitle;
		std::wstring _errorUserMessage;
	};
}
