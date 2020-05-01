#include "FBError.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winsdkfb
{
	FBError::FBError()
		: _message()
		, _type()
		, _code(0)
		, _subcode(0)
		, _errorUserTitle()
		, _errorUserMessage()
	{
		;
	}

	hstring FBError::Message()
	{
		return  _message.c_str();
	}

	hstring FBError::Type()
	{
		return  _type.c_str();
	}

	int32_t FBError::Code()
	{
		return _code;
	}

	int32_t FBError::Subcode()
	{
		return _subcode;
	}

	hstring FBError::ErrorUserTitle()
	{
		return  _errorUserTitle.c_str();
	}

	hstring FBError::ErrorUserMessage()
	{
		return  _errorUserMessage.c_str();
	}

	std::shared_ptr<FBError> FBError::FromUri(Uri const& ResponseUri)
	{
		auto result = std::make_shared<FBError>();
		bool foundCode = false;
		bool foundDescription = false;
		bool foundMessage = false;
		bool foundReason = false;
		int code = 0;
		hstring reason;
		hstring description;
		hstring message;
		hstring query = ResponseUri.Query();

		if (!query.empty())
		{
			auto decoder = WwwFormUrlDecoder(ResponseUri.Query());

			for (unsigned int i = 0; i < decoder.Size(); i++)
			{
				IWwwFormUrlDecoderEntry entry = decoder.GetAt(i);
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
				result->_code = code;
				result->_type = reason;
				if (foundDescription)
				{
					result->_message = description;
				}
				else
				{
					result->_message = message;
				}
			}
		}

		return result;
	}

	std::shared_ptr<FBError> FBError::FromJson(hstring const& JsonText)
	{
		std::shared_ptr<FBError> result = nullptr;
		JsonValue val{ nullptr };

		if (JsonValue::TryParse(JsonText, val))
		{
			JsonObject obj = val.GetObject();
			if (obj.HasKey(L"error")) {
				obj = obj.GetNamedObject(L"error");
			}

			result = std::make_shared<FBError>();
			int found = 0;

			for (auto&& current : obj)
			{
				winrt::hstring key = current.Key();

				if (key == L"message")
				{
					found++;
					result->_message = current.Value().GetString();
				}
				else if (key == L"type")
				{
					found++;
					result->_type = current.Value().GetString();
				}
				else if (key == L"code")
				{
					found++;
					result->_code = static_cast<int>(current.Value().GetNumber());
				}
				else if (key == L"error_subcode")
				{
					found++;
					result->_subcode = static_cast<int>(current.Value().GetNumber());
				}
				else if (key == L"error_user_title")
				{
					found++;
					result->_errorUserTitle = current.Value().GetString();
				}
				else if (key == L"error_user_msg")
				{
					found++;
					result->_errorUserMessage = current.Value().GetString();
				}
			}

			if (!found) {
				result = nullptr;
			}
		}
		return result;
	}

	FBError::FBError(int32_t Code, hstring const& Type, hstring const& Message)
		: _message(Message)
		, _type(Type)
		, _code(Code)
		, _subcode(0)
		, _errorUserTitle()
		, _errorUserMessage()
	{
		;
	}
}
