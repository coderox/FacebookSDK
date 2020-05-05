#include "FBSingleValue.h"
#include "SDKMessage.h"
#include "HttpManager.h"

#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace concurrency;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winsdkfb
{
	FBSingleValue::FBSingleValue(hstring const& request, PropertySet const& parameters, winsdkfb::JsonClassFactory objectFactory)
		: _request(request)
		, _parameters(parameters)
		, _objectFactory(objectFactory)
	{

	}

	task<FBResult> FBSingleValue::GetAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Get);
	}

	task<FBResult> FBSingleValue::PostAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Post);
	}

	task<FBResult> FBSingleValue::DeleteAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Delete);
	}

	FBResult FBSingleValue::ConsumeSingleValue(hstring jsonText)
	{
		FBResult result;

		JsonValue val{ nullptr };
		FBError err;
		FBResult item;

		if (JsonValue::TryParse(jsonText, val))
		{
			if (val.ValueType() == JsonValueType::Object)
			{
				//TODO: Check for error here first.  User's object serializer may
				//produce a false positive.
				err = FBError::FromJson(jsonText);
				if (err.Code()) {
					item = _objectFactory(jsonText);
					if (!item.Succeeded()) {
						auto obj = val.GetObject();
						for (auto&& current : obj)
						{
							auto key = current.Key();
							if (key == L"error")
							{
								err = FBError::FromJson(current.Value().GetString());
								break;
							}
							else if (key == L"data")
							{
								if (current.Value().ValueType() != JsonValueType::Object) {
									throw hresult_invalid_argument(SDKMessageBadObject);
								}
								item = _objectFactory(current.Value().GetString());
								if (!item.Succeeded()) {
									throw hresult_invalid_argument(SDKMessageBadObject);
								}
							}
						}
					}
				}
			}

			if (err.Code()) {
				result = FBResult(err);
			}
			else {
				throw hresult_invalid_argument(SDKMessageNoData);
			}
		}

		return result;
	}

	task<FBResult> FBSingleValue::MakeHttpRequest(HttpMethod httpMethod)
	{
		if (_parameters == nullptr) {
			_parameters = PropertySet();
		}

		//auto fields = _parameters.Lookup(L"fields");

		hstring responseString;
		switch (httpMethod)
		{
		case HttpMethod::Get:
			responseString = co_await HttpManager::Instance()->GetTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		case HttpMethod::Post:
			responseString = co_await HttpManager::Instance()->PostTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		case HttpMethod::Delete:
			responseString = co_await HttpManager::Instance()->DeleteTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		default:
			//OutputDebugString(L"FBSingleValue::MakeHttpRequest recieved unknown HttpMethod value\n");
			responseString = L"";
			break;
		}

		if (responseString.empty()) {
			auto error = FBError(0, L"HTTP request failed", L"Unable to receive response");
			co_return FBResult(error);
		}
		else {
			co_return(ConsumeSingleValue(responseString));
		}
	}
}
