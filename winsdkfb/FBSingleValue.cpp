#include "pch.h"
#include "FBSingleValue.h"
#include "SDKMessage.h"
#include <ppltasks.h>

using namespace winrt;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;

namespace winrt::winsdkfb::implementation
{
	FBSingleValue::FBSingleValue(hstring const& request, PropertySet const& parameters, winsdkfb::JsonClassFactory const& objectFactory)
		: _request(request)
		, _parameters(parameters)
		, _objectFactory(objectFactory)
		, _result(nullptr)
	{

	}

	IAsyncOperation<winsdkfb::FBResult> FBSingleValue::GetAsync()
	{
		return MakeHttpRequest(::winsdkfb::HttpMethod::Get);
	}

	IAsyncOperation<winsdkfb::FBResult> FBSingleValue::PostAsync()
	{
		return MakeHttpRequest(::winsdkfb::HttpMethod::Post);
	}

	IAsyncOperation<winsdkfb::FBResult> FBSingleValue::DeleteAsync()
	{
		return MakeHttpRequest(::winsdkfb::HttpMethod::Delete);
	}

	winsdkfb::FBResult FBSingleValue::ConsumeSingleValue(hstring jsonText)
	{
		winsdkfb::FBResult result{ nullptr };

		JsonValue val{ nullptr };
		winsdkfb::FBError err{ nullptr };
		IInspectable item{ nullptr };

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

			if (item) {
				result = winrt::make<FBResult>(item);
			}
			else if (err) {
				result = winrt::make<FBResult>(err);
			}
			else {
				throw hresult_invalid_argument(SDKMessageNoData);
			}
		}

		return result.as<winrt::winsdkfb::FBResult>();
	}

	IAsyncOperation<winsdkfb::FBResult> FBSingleValue::MakeHttpRequest(::winsdkfb::HttpMethod httpMethod)
	{
		if (_parameters == nullptr) {
			_parameters = PropertySet();
		}

		//auto fields = _parameters.Lookup(L"fields");

		hstring responseString;
		switch (httpMethod)
		{
		case ::winsdkfb::HttpMethod::Get:
			responseString = co_await HttpManager::Instance().GetTaskAsync(_request, _parameters.GetView());
			break;
		case ::winsdkfb::HttpMethod::Post:
			responseString = co_await HttpManager::Instance().PostTaskAsync(_request, _parameters.GetView());
			break;
		case ::winsdkfb::HttpMethod::Delete:
			responseString = co_await HttpManager::Instance().DeleteTaskAsync(_request, _parameters.GetView());
			break;
		default:
			OutputDebugString(L"FBSingleValue::MakeHttpRequest recieved unknown HttpMethod value\n");
			responseString = L"";
			break;
		}

		if (responseString.empty()) {
			co_return(winsdkfb::FBResult(winsdkfb::FBError(0, L"HTTP request failed", L"Unable to receive response")));
		}
		else {
			co_return(ConsumeSingleValue(responseString));
		}
	}
}
