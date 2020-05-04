#include "FBPageCategory.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBPageCategory::Id()
    {
        return _id;
    }
    void FBPageCategory::Id(hstring const& value)
    {
        _id = value;
    }

    hstring FBPageCategory::Name()
    {
        return _name;
    }
    void FBPageCategory::Name(hstring const& value)
    {
        _name = value;
    }

    FBPageCategory FBPageCategory::FromJson(
        hstring const& JsonText 
        )
    {
        FBPageCategory result;
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
                        result.Id(current.Value().GetString());
                    }
                    else if (key == L"name")
                    {
                        found++;
                        result.Name(current.Value().GetString());
                    }
                }

				if(found){
					result._succeeded = true;
				}
            }
        }
        return result;
    }
}



