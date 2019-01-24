#include "pch.h"
#include "FacebookError.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winrt::FacebookSDK::implementation
{
	FacebookError::FacebookError()
		: _message()
		, _type()
		, _code(0)
		, _subcode(0)
		, _errorUserTitle()
		, _errorUserMessage()
	{
		;
	}

	hstring FacebookError::Message()
	{
		return  _message.c_str();
	}

	hstring FacebookError::Type()
	{
		return  _type.c_str();
	}

	int32_t FacebookError::Code()
	{
		return _code;
	}

	int32_t FacebookError::Subcode()
	{
		return _subcode;
	}

	hstring FacebookError::ErrorUserTitle()
	{
		return  _errorUserTitle.c_str();
	}

	hstring FacebookError::ErrorUserMessage()
	{
		return  _errorUserMessage.c_str();
	}

	FacebookSDK::FacebookError FacebookError::FromUri(Uri const& ResponseUri)
	{
		auto result = winrt::make_self<FacebookError>();
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

		return result.as<winrt::FacebookSDK::FacebookError>();
	}

	FacebookSDK::FacebookError FacebookError::FromJson(hstring const& JsonText)
	{
		com_ptr<FacebookError> result{ nullptr };
		JsonValue val{ nullptr };

		if (JsonValue::TryParse(JsonText, val))
		{
			JsonObject obj = val.GetObject();
			if (obj.HasKey(L"error")) {
				obj = obj.GetNamedObject(L"error");
			}

			result = winrt::make_self<FacebookError>();
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
		return result.as<winrt::FacebookSDK::FacebookError>();
	}

	FacebookError::FacebookError(int32_t Code, hstring const& Type, hstring const& Message)
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
