#include "FBSingleValue.h"
#include "SDKMessage.h"
#include "HttpManager.h"

//#if _DEBUG
//#include <Windows.h>
//#endif

#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace concurrency;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winsdkfb
{
	FBSingleValue::FBSingleValue(wstring const& request, unordered_map<wstring, wstring> parameters, winsdkfb::JsonClassFactory objectFactory)
		: _request(request)
		, _parameters(parameters)
		, _objectFactory(objectFactory)
	{

	}

	task<FBResult> FBSingleValue::GetAsync() const {
		return MakeHttpRequest(winsdkfb::HttpMethod::Get);
	}

	task<FBResult> FBSingleValue::PostAsync() const {
		return MakeHttpRequest(winsdkfb::HttpMethod::Post);
	}

	task<FBResult> FBSingleValue::DeleteAsync() const {
		return MakeHttpRequest(winsdkfb::HttpMethod::Delete);
	}

	FBResult FBSingleValue::ConsumeSingleValue(wstring jsonText) const {
		FBResult result;

		JsonValue val{ nullptr };
		FBError err;
		std::any item;

		if (JsonValue::TryParse(jsonText, val))
		{
			if (val.ValueType() == JsonValueType::Object)
			{
				//TODO: Check for error here first.  User's object serializer may
				//produce a false positive.
				err = FBError::FromJson(jsonText);
				if (err.Code() == 0) {
					item = _objectFactory(jsonText);
					if (!item.has_value()) {
						auto obj = val.GetObject();
						for (auto&& current : obj)
						{
							auto key = current.Key();
							if (key == L"error")
							{
								err = FBError::FromJson(current.Value().GetString().c_str());
								break;
							}
							else if (key == L"data")
							{
								if (current.Value().ValueType() != JsonValueType::Object) {
									throw hresult_invalid_argument(SDKMessageBadObject);
								}
								item = _objectFactory(current.Value().GetString().c_str());
								if (!item.has_value()) {
									throw hresult_invalid_argument(SDKMessageBadObject);
								}
							}
						}
					}
				}
			}

			if (item.has_value()) {
				result = std::any_cast<FBResult>(item);
			} else if (err.Code() != 0) {
				result = FBResult(err);
			} else {
				throw hresult_invalid_argument(SDKMessageNoData);
			}
		}

		return result;
	}

	task<FBResult> FBSingleValue::MakeHttpRequest(HttpMethod httpMethod) const {
		hstring responseString;
		switch (httpMethod)
		{
		case HttpMethod::Get:
			responseString = co_await HttpManager::Instance()->GetTaskAsync(_request, _parameters);
			break;
		case HttpMethod::Post:
			responseString = co_await HttpManager::Instance()->PostTaskAsync(_request, _parameters);
			break;
		case HttpMethod::Delete:
			responseString = co_await HttpManager::Instance()->DeleteTaskAsync(_request, _parameters);
			break;
		default:
#if _DEBUG
			//OutputDebugString(L"FBSingleValue::MakeHttpRequest received unknown HttpMethod value\n");
#endif
			responseString = L"";
			break;
		}

		if (responseString.empty()) {
			auto error = FBError(0, L"HTTP request failed", L"Unable to receive response");
			co_return FBResult(error);
		}
		else {
			co_return(ConsumeSingleValue(responseString.c_str()));
		}
	}
}
