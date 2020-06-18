#include "Graph/FBGroup.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBGroup::Id()
    {
        return _id;
    }
    void FBGroup::Id(hstring const& value)
    {
        _id = value;
    }

    hstring FBGroup::Description()
    {
        return _description;
    }
    void FBGroup::Description(hstring const& value)
    {
        _description = value;
    }

    hstring FBGroup::Email()
    {
        return _email;
    }
    void FBGroup::Email(hstring const& value)
    {
        _email = value;
    }

    hstring FBGroup::Icon()
    {
        return _icon;
    }
    void FBGroup::Icon(hstring const& value)
    {
        _icon = value;
    }

    hstring FBGroup::Link()
    {
        return _link;
    }
    void FBGroup::Link(hstring const& value)
    {
        _link = value;
    }

    hstring FBGroup::Name()
    {
        return _name;
    }
    void FBGroup::Name(hstring const& value)
    {
        _name = value;
    }

    hstring FBGroup::Privacy()
    {
        return _privacy;
    }
    void FBGroup::Privacy(hstring const& value)
    {
        _privacy = value;
    }

    any FBGroup::FromJson(
        hstring const& JsonText 
        )
    {
        any result;
        FBGroup group;
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
                    if  (key == L"id")
                    {
                        found++;
                        group.Id(current.Value().GetString());
                    }
                    else if (key == L"description")
                    {
                        found++;
                        group.Description(current.Value().GetString());
                    }
                    else if (key == L"email")
                    {
                        found++;
                        group.Email(current.Value().GetString());
                    }
                    else if (key == L"icon")
                    {
                        found++;
                        group.Icon(current.Value().GetString());
                    }
                    else if (key == L"link")
                    {
                        found++;
                        group.Link(current.Value().GetString());
                    }
                    else if (key == L"name")
                    {
                        found++;
                        group.Name(current.Value().GetString());
                    }
                    else if (key == L"privacy")
                    {
                        found++;
                        group.Privacy(current.Value().GetString());
                    }
                }

				if(found){
					result = group;
				}
            }
        }
        return result;
    }
}
