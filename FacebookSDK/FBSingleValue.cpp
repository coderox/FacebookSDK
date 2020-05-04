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
		, _result(nullptr)
	{

	}

	task<shared_ptr<FBResult>> FBSingleValue::GetAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Get);
	}

	task<shared_ptr<FBResult>> FBSingleValue::PostAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Post);
	}

	task<shared_ptr<FBResult>> FBSingleValue::DeleteAsync()
	{
		return MakeHttpRequest(winsdkfb::HttpMethod::Delete);
	}

	shared_ptr<FBResult> FBSingleValue::ConsumeSingleValue(hstring jsonText)
	{
		shared_ptr<FBResult> result{ nullptr };

		JsonValue val{ nullptr };
		shared_ptr<FBError> err{ nullptr };
		shared_ptr<FBResult> item{ nullptr };

		if (JsonValue::TryParse(jsonText, val))
		{
			if (val.ValueType() == JsonValueType::Object)
			{
				//TODO: Check for error here first.  User's object serializer may
				//produce a false positive.
				err = FBError::FromJson(jsonText);
				if (!err) {
					item = _objectFactory(jsonText);
					if (!item) {
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
								if (!item) {
									throw hresult_invalid_argument(SDKMessageBadObject);
								}
							}
						}
					}
				}
			}

			if (err) {
				result = make_shared<FBResult>(err);
			}
			else {
				throw hresult_invalid_argument(SDKMessageNoData);
			}
		}

		return result;
	}

	task<shared_ptr<FBResult>> FBSingleValue::MakeHttpRequest(winsdkfb::HttpMethod httpMethod)
	{
		if (_parameters == nullptr) {
			_parameters = PropertySet();
		}

		//auto fields = _parameters.Lookup(L"fields");

		hstring responseString;
		switch (httpMethod)
		{
		case winsdkfb::HttpMethod::Get:
			responseString = co_await HttpManager::Instance()->GetTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		case ::winsdkfb::HttpMethod::Post:
			responseString = co_await HttpManager::Instance()->PostTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		case ::winsdkfb::HttpMethod::Delete:
			responseString = co_await HttpManager::Instance()->DeleteTaskAsync(_request.c_str(), _parameters.GetView());
			break;
		default:
			//OutputDebugString(L"FBSingleValue::MakeHttpRequest recieved unknown HttpMethod value\n");
			responseString = L"";
			break;
		}

		if (responseString.empty()) {
			auto error = make_shared<winsdkfb::FBError>(0, L"HTTP request failed", L"Unable to receive response");
			co_return(make_shared<winsdkfb::FBResult>(error));
		}
		else {
			co_return(ConsumeSingleValue(responseString));
		}
	}
}
