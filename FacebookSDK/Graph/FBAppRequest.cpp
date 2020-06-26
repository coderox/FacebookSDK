#include "Graph/FBAppRequest.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    wstring FBAppRequest::ActionType() const
    {
        return _action_type;
    }
	void FBAppRequest::ActionType(wstring const& value)
    {
        _action_type = value;
    }

    wstring FBAppRequest::CreatedTime() const
    {
        return _created_time;
    }
	void FBAppRequest::CreatedTime(wstring const& value)
    {
        _created_time = value;
    }

    wstring FBAppRequest::Data() const
    {
        return _data;
    }
	void FBAppRequest::Data(wstring const& value)
    {
        _data = value;
    }

    winsdkfb::Graph::FBUser FBAppRequest::From() const
    {
        return _from;
    }
    void FBAppRequest::From(winsdkfb::Graph::FBUser value)
    {
        _from = value;
    }

    wstring FBAppRequest::Id() const
    {
        return _id;
    }
	void FBAppRequest::Id(wstring const& value)
    {
        _id = value;
    }

    wstring FBAppRequest::Message() const
    {
        return _message;
    }
	void FBAppRequest::Message(wstring const& value)
    {
        _message = value;
    }

    winsdkfb::Graph::FBUser FBAppRequest::To() const
    {
        return _to;
    }
    void FBAppRequest::To(winsdkfb::Graph::FBUser value)
    {
        _to = value;
    }

    any FBAppRequest::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBAppRequest request;
        int found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(jsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
                JsonObject obj = val.GetObject();
                for (auto&& current : obj)
                {
                    std::wstring key = current.Key().c_str();
                    if  (key == L"action_type")
                    {
                        found++;
                        request._action_type = current.Value().GetString();
                    }
                    else if (key == L"created_time")
                    {
                        found++;
                        request._created_time = current.Value().GetString();
                    }
                    else if (key == L"data")
                    {
                        found++;
                        request._data = current.Value().GetString();
                    }
                    else if (key == L"from")
                    {
                        found++;
                        request._from = std::any_cast<FBUser>(FBUser::FromJson(current.Value().Stringify().c_str()));
                    }
                    else if (key == L"id")
                    {
                        found++;
                        request._id = current.Value().GetString();
                    }
                    else if (key == L"message")
                    {
                        found++;
                        request._message = current.Value().GetString();
                    }
                    else if (key == L"to")
                    {
                        found++;
                        request._to = std::any_cast<FBUser>(FBUser::FromJson(current.Value().Stringify().c_str()));
                    }
                }

				if(found) {
					result = request;
				}
            }
        }
        return result;
    }
}


