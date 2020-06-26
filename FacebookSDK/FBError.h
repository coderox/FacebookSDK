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
		ErrorSubCodeAppNotAuthorized = 458,
		ErrorSubCodeSessionInvalidated = 466
	};

	struct FBError
	{
	public:
		static FBError FromUri(winrt::Windows::Foundation::Uri const& responseUri);
		static FBError FromJson(std::wstring const& jsonText);

		FBError() = default;
		FBError(int32_t code, std::wstring type, std::wstring message);
		FBError(std::nullptr_t) noexcept {}

		std::wstring Message() const;
		std::wstring Type() const;
		int32_t Code() const;
		int32_t SubCode() const;
		std::wstring ErrorUserTitle() const;
		std::wstring ErrorUserMessage() const;

	private:

		std::wstring _message;
		std::wstring _type;
		int32_t _code = 0;
		int32_t _subcode = 0;
		std::wstring _errorUserTitle;
		std::wstring _errorUserMessage;
	};
}
