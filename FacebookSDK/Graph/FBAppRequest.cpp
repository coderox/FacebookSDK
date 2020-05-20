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
    hstring FBAppRequest::ActionType()
    {
        return _action_type;
    }
    void FBAppRequest::ActionType(hstring const& value)
    {
        _action_type = value;
    }

    hstring FBAppRequest::CreatedTime()
    {
        return _created_time;
    }
    void FBAppRequest::CreatedTime(hstring const& value)
    {
        _created_time = value;
    }

    hstring FBAppRequest::Data()
    {
        return _data;
    }
    void FBAppRequest::Data(hstring const& value)
    {
        _data = value;
    }

    winsdkfb::Graph::FBUser FBAppRequest::From()
    {
        return _from;
    }
    void FBAppRequest::From(winsdkfb::Graph::FBUser value)
    {
        _from = value;
    }

    hstring FBAppRequest::Id()
    {
        return _id;
    }
    void FBAppRequest::Id(hstring const& value)
    {
        _id = value;
    }

    hstring FBAppRequest::Message()
    {
        return _message;
    }
    void FBAppRequest::Message(hstring const& value)
    {
        _message = value;
    }

    winsdkfb::Graph::FBUser FBAppRequest::To()
    {
        return _to;
    }
    void FBAppRequest::To(winsdkfb::Graph::FBUser value)
    {
        _to = value;
    }

    winsdkfb::Graph::FBAppRequest FBAppRequest::FromJson(
        hstring const& JsonText 
        )
    {
        FBAppRequest result;
        int found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(JsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
                JsonObject obj = val.GetObject();
                for (auto&& current : obj)
                {
                    winrt::hstring key = current.Key();
                    if  (key == L"action_type")
                    {
                        found++;
                        result.ActionType(current.Value().GetString());
                    }
                    else if (key == L"created_time")
                    {
                        found++;
                        result.CreatedTime(current.Value().GetString());
                    }
                    else if (key == L"data")
                    {
                        found++;
                        result.Data(current.Value().GetString());
                    }
                    else if (key == L"from")
                    {
                        found++;
                        result.From(std::any_cast<FBUser>(FBUser::FromJson(current.Value().Stringify())));
                    }
                    else if (key == L"id")
                    {
                        found++;
                        result.Id(current.Value().GetString());
                    }
                    else if (key == L"message")
                    {
                        found++;
                        result.Message(current.Value().GetString());
                    }
                    else if (key == L"to")
                    {
                        found++;
                        result.To(std::any_cast<FBUser>(FBUser::FromJson(current.Value().Stringify())));
                    }
                }

				//if(found){
				//	result._succeeded = true;
				//}
            }
        }
        return result;
    }
}


