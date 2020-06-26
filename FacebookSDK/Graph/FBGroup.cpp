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
    wstring FBGroup::Id() const {
        return _id;
    }
	void FBGroup::Id(wstring const& value)
    {
        _id = value;
    }

    wstring FBGroup::Description() const {
        return _description;
    }
	void FBGroup::Description(wstring const& value)
    {
        _description = value;
    }

    wstring FBGroup::Email() const {
        return _email;
    }
	void FBGroup::Email(wstring const& value)
    {
        _email = value;
    }

    wstring FBGroup::Icon() const {
        return _icon;
    }
	void FBGroup::Icon(wstring const& value)
    {
        _icon = value;
    }

    wstring FBGroup::Link() const {
        return _link;
    }
	void FBGroup::Link(wstring const& value)
    {
        _link = value;
    }

    wstring FBGroup::Name() const {
        return _name;
    }
	void FBGroup::Name(wstring const& value)
    {
        _name = value;
    }

    wstring FBGroup::Privacy() const {
        return _privacy;
    }
	void FBGroup::Privacy(wstring const& value)
    {
        _privacy = value;
    }

    any FBGroup::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBGroup group;
        auto found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(jsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
	            auto obj = val.GetObject();
                for (auto&& current : obj)
                {
                    auto key = current.Key();
                    if  (key == L"id")
                    {
                        found++;
                        group.Id(current.Value().GetString().c_str());
                    }
                    else if (key == L"description")
                    {
                        found++;
						group.Description(current.Value().GetString().c_str());
                    }
                    else if (key == L"email")
                    {
                        found++;
						group.Email(current.Value().GetString().c_str());
                    }
                    else if (key == L"icon")
                    {
                        found++;
						group.Icon(current.Value().GetString().c_str());
                    }
                    else if (key == L"link")
                    {
                        found++;
						group.Link(current.Value().GetString().c_str());
                    }
                    else if (key == L"name")
                    {
                        found++;
						group.Name(current.Value().GetString().c_str());
                    }
                    else if (key == L"privacy")
                    {
                        found++;
						group.Privacy(current.Value().GetString().c_str());
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
