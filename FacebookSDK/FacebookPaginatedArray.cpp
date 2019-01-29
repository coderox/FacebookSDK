#include "pch.h"
#include "Utilities.h"
#include "SDKMessage.h"
#include "FacebookPaginatedArray.h"
#include "FacebookResult.h"
#include "FacebookSession.h"
#include "HttpManager.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;

namespace winrt::FacebookSDK::implementation
{
	FacebookPaginatedArray::FacebookPaginatedArray(hstring const& request, PropertySet const& parameters, FacebookSDK::JsonClassFactory const& objectFactory)
		: _current(nullptr)
		, _currentDataString(L"")
		, _request(request)
		, _parameters(parameters)
		, _objectFactory(objectFactory)
	{
		if (_parameters == nullptr) {
			_parameters = PropertySet();
		}
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookPaginatedArray::FirstAsync()
	{
		return GetPageAsync(_request);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookPaginatedArray::NextAsync()
	{
		if (!HasNext())
		{
			co_return make<FacebookResult>(FacebookError(0, L"Invalid SDK call", L"No next page"));
		}

		return GetPageAsync(_paging.Next().c_str());
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookPaginatedArray::PreviousAsync()
	{
		if (!HasPrevious())
		{
			co_return make<FacebookResult>(FacebookError(0, L"Invalid SDK call", L"No previous page"));
		}

		return GetPageAsync(_paging.Previous().c_str());
	}

	IVectorView<IInspectable> FacebookPaginatedArray::Current()
	{
		if (!HasCurrent())
		{
			throw hresult_invalid_argument(SDKMessageBadCall);
		}

		return _current;
	}

	hstring FacebookPaginatedArray::CurrentDataString()
	{
		if (!HasCurrent())
		{
			throw hresult_invalid_argument(SDKMessageBadCall);
		}

		return _currentDataString.c_str();
	}

	bool FacebookPaginatedArray::HasCurrent()
	{
		return (_current != nullptr);
	}

	bool FacebookPaginatedArray::HasNext()
	{
		return(_paging && (!_paging.Next().empty()));
	}

	bool FacebookPaginatedArray::HasPrevious()
	{
		return(_paging && (!_paging.Previous().empty()));
	}

	IVectorView<IInspectable> FacebookPaginatedArray::ObjectArrayFromWebResponse(hstring const& Response, FacebookSDK::JsonClassFactory const& classFactory)
	{
		IVectorView<IInspectable> result = { nullptr };
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
					result = ObjectArrayFromJsonArray(it.Current().Value().GetArray(), classFactory);
				}
			}
		}

		return result;
	}

	IVectorView<Windows::Foundation::IInspectable> FacebookPaginatedArray::ObjectArrayFromJsonArray(
		JsonArray values,
		FacebookSDK::JsonClassFactory classFactory
	) {
		IVector<IInspectable> result = { winrt::single_threaded_vector<IInspectable>() };

		for (auto const& current : values)
		{
			auto jsonText = current.Stringify();
			auto item = classFactory(jsonText);
			if (item == nullptr) {
				throw hresult_invalid_argument(SDKMessageBadObject);
			}
			result.Append(item);
		}

		return result.GetView();
	}

	FacebookSDK::FacebookResult FacebookPaginatedArray::ConsumePagedResponse(
		wstring jsonText
	) {
		FacebookSDK::FacebookResult result = nullptr;
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
						auto err = FacebookError::FromJson(it.Current().Value().Stringify());
						result = make<FacebookResult>(err);
						foundError = true;
						break;
					}
					else if (compare_ordinal(it.Current().Key().c_str(), L"paging") == 0)
					{
						_paging = Graph::FBPaging::FromJson(it.Current().Value().Stringify()).as<Graph::FBPaging>();
						if (_paging)
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
						_current = ObjectArrayFromJsonArray(it.Current().Value().GetArray(), _objectFactory);
						if (_current)
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
				result = make<FacebookResult>(_current);
			}
		}

		return result;
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookPaginatedArray::GetPageAsync(wstring path) {
		hstring responseString = co_await HttpManager::Instance().GetTaskAsync(path, _parameters.GetView());
		if (responseString.empty())
		{
			co_return make<FacebookResult>(FacebookError(0, L"HTTP request failed", L"unable to receive response"));
		}
		else
		{
			co_return ConsumePagedResponse(responseString.c_str());
		}
	}
}
