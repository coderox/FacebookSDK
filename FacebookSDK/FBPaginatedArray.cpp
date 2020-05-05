#include "FBPaginatedArray.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Http.Filters.h>

#include "SDKMessage.h"
#include "HttpManager.h"

using namespace std;
using namespace winrt;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;

namespace winsdkfb
{
	FBPaginatedArray::FBPaginatedArray(hstring const& request, PropertySet const& parameters, winsdkfb::JsonClassFactory const& objectFactory)
		: _currentDataString(L"")
		, _request(request)
		, _parameters(parameters)
		, _objectFactory(objectFactory)
	{
		if (_parameters == nullptr) {
			_parameters = PropertySet();
		}
	}

	task<winsdkfb::FBResult> FBPaginatedArray::FirstAsync()
	{
		return GetPageAsync(_request);
	}

	task<winsdkfb::FBResult> FBPaginatedArray::NextAsync()
	{
		if (!HasNext())
		{
			co_return FBResult(FBError(0, L"Invalid SDK call", L"No next page"));
		}
	
		co_return co_await GetPageAsync(_paging.Next().c_str());
	}

	task<winsdkfb::FBResult> FBPaginatedArray::PreviousAsync()
	{
		if (!HasPrevious())
		{
			co_return FBResult(FBError(0, L"Invalid SDK call", L"No previous page"));
		}

		co_return co_await GetPageAsync(_paging.Previous().c_str());
	}

	vector<FBResult> FBPaginatedArray::Current()
	{
		if (!HasCurrent())
		{
			throw hresult_invalid_argument(SDKMessageBadCall);
		}

		return _current;
	}

	hstring FBPaginatedArray::CurrentDataString()
	{
		if (!HasCurrent())
		{
			throw hresult_invalid_argument(SDKMessageBadCall);
		}

		return _currentDataString.c_str();
	}

	bool FBPaginatedArray::HasCurrent()
	{
		return !_current.empty();
	}

	bool FBPaginatedArray::HasNext()
	{
		return(!_paging.Next().empty());
	}

	bool FBPaginatedArray::HasPrevious()
	{
		return(!_paging.Previous().empty());
	}

	vector<FBResult> FBPaginatedArray::VectorFromWebResponse(hstring const& Response, winsdkfb::JsonClassFactory const& classFactory)
	{
		vector<FBResult> result;
		JsonObject rootObject = nullptr;

		if (JsonObject::TryParse(Response, rootObject))
		{
			IIterator<IKeyValuePair<hstring, IJsonValue>> it = nullptr;
			for (it = rootObject.First(); it.HasCurrent(); it.MoveNext())
			{
				hstring key = it.Current().Key();
				JsonValueType t = it.Current().Value().ValueType();

				if ((compare_ordinal(key.c_str(), L"data") == 0) &&
					(t == JsonValueType::Array))
				{
					result = VectorFromJsonArray(it.Current().Value().GetArray(), classFactory);
				}
			}
		}

		return result;
	}

	vector<FBResult> FBPaginatedArray::VectorFromJsonArray(
		JsonArray values,
		winsdkfb::JsonClassFactory classFactory
	) {
		vector<FBResult> result;

		for (auto const& current : values)
		{
			auto jsonText = current.Stringify();
			auto item = classFactory(jsonText);
			if (!item.Succeeded()) {
				throw hresult_invalid_argument(SDKMessageBadObject);
			}
			result.push_back(item);
		}

		return result;
	}

	winsdkfb::FBResult FBPaginatedArray::ConsumePagedResponse(
		wstring jsonText
	) {
		winsdkfb::FBResult result;
		JsonValue value{ nullptr };
		bool foundPaging = false;
		bool foundData = false;
		bool foundError = false;

		if (JsonValue::TryParse(jsonText, value))
		{
			if (value.ValueType() == JsonValueType::Object)
			{
				JsonObject obj = value.GetObject();

				IIterator<IKeyValuePair<hstring, IJsonValue>> it = nullptr;
				for (it = obj.First(); it.HasCurrent(); it.MoveNext())
				{
					if (compare_ordinal(it.Current().Key().c_str(), L"error") == 0)
					{
						auto err = FBError::FromJson(it.Current().Value().Stringify());
						result = FBResult(err);
						foundError = true;
						break;
					}
					else if (compare_ordinal(it.Current().Key().c_str(), L"paging") == 0)
					{
						_paging = Graph::FBPaging::FromJson(it.Current().Value().Stringify());
						if (_paging.Succeeded())
						{
							foundPaging = true;
						}
					}
					else if (compare_ordinal(it.Current().Key().c_str(), L"data") == 0)
					{
						if (it.Current().Value().ValueType() != JsonValueType::Array)
						{
							throw hresult_invalid_argument(SDKMessageBadObject);
						}

						_currentDataString = it.Current().Value().as<IStringable>().ToString();
						_current = VectorFromJsonArray(it.Current().Value().GetArray(), _objectFactory);
						if (!_current.empty())
						{
							foundData = true;
						}
					}
				}
			}

			// If all data for the array fits in one page, we don't get a 'paging'
			// object.  We should always get a data object, though. Note that a FB
			// error should not result in a thrown exception!
			if (!(foundError || foundData))
			{
				throw hresult_invalid_argument(SDKMessageBadObject);
			}

			if (!foundError)
			{
				result = FBVectorOfResults(_current);
			}
		}

		return result;
	}

	task<winsdkfb::FBResult> FBPaginatedArray::GetPageAsync(hstring path) {
		hstring responseString = co_await HttpManager::Instance()->GetTaskAsync(path, _parameters.GetView());
		if (responseString.empty())
		{
			co_return FBResult(FBError(0, L"HTTP request failed", L"unable to receive response"));
		}
		else
		{
			co_return ConsumePagedResponse(responseString.c_str());
		}
	}
}
