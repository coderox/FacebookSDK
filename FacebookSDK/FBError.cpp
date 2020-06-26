#include "FBError.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winsdkfb
{
	wstring FBError::Message() const {
		return  _message;
	}

	wstring FBError::Type() const {
		return  _type;
	}

	int32_t FBError::Code() const {
		return _code;
	}

	int32_t FBError::SubCode() const {
		return _subcode;
	}

	wstring FBError::ErrorUserTitle() const	{
		return  _errorUserTitle;
	}

	wstring FBError::ErrorUserMessage() const {
		return  _errorUserMessage;
	}

	FBError FBError::FromUri(Uri const& responseUri)
	{
		FBError result;
		auto foundCode = false;
		auto foundDescription = false;
		auto foundMessage = false;
		auto foundReason = false;
		auto code = 0;
		hstring reason;
		hstring description;
		hstring message;
		const auto query = responseUri.Query();

		if (!query.empty())
		{
			const auto decoder = WwwFormUrlDecoder(query);

			for (unsigned int i = 0; i < decoder.Size(); i++)
			{
				auto entry = decoder.GetAt(i);
				if (entry.Name() == L"error_code")
				{
					foundCode = true;
					code = _wtoi(entry.Value().data());
				}
				else if (entry.Name() == L"error_description")
				{
					foundDescription = true;
					description = entry.Value();
				}
				else if (entry.Name() == L"error_message")
				{
					foundMessage = true;
					message = entry.Value();
				}
				else if (entry.Name() == L"error_reason")
				{
					foundReason = true;
					reason = entry.Value();
				}
			}

			if (foundCode || foundDescription || foundMessage || foundReason)
			{
				result._code = code;
				result._type = reason;
				if (foundDescription)
				{
					result._message = description;
				}
				else
				{
					result._message = message;
				}
			}
		}

		return result;
	}

	FBError FBError::FromJson(wstring const& jsonText)
	{
		FBError result;
		JsonValue val{ nullptr };

		if (JsonValue::TryParse(jsonText, val))
		{
			auto obj = val.GetObject();
			if (obj.HasKey(L"error")) {
				obj = obj.GetNamedObject(L"error");
			}

			auto found = 0;

			for (auto&& current : obj)
			{
				auto key = current.Key();

				if (key == L"message")
				{
					found++;
					result._message = current.Value().GetString();
				}
				else if (key == L"type")
				{
					found++;
					result._type = current.Value().GetString();
				}
				else if (key == L"code")
				{
					found++;
					result._code = static_cast<int>(current.Value().GetNumber());
				}
				else if (key == L"error_subcode")
				{
					found++;
					result._subcode = static_cast<int>(current.Value().GetNumber());
				}
				else if (key == L"error_user_title")
				{
					found++;
					result._errorUserTitle = current.Value().GetString();
				}
				else if (key == L"error_user_msg")
				{
					found++;
					result._errorUserMessage = current.Value().GetString();
				}
			}

			if (found == 0) {
				result = FBError(0, L"", L"");
			}
		}

		return result;
	}

	FBError::FBError(int32_t code, wstring type, wstring message)
		: _message(message)
		, _type(type)
		, _code(code)
	{
		
	}
}
