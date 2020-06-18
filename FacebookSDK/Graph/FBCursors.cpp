#include "Graph/FBCursors.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBCursors::After()
    {
        return _after;
    }
    void FBCursors::After(hstring const& value)
    {
        _after = value;
    }

    hstring FBCursors::Before()
    {
        return _before;
    }
    void FBCursors::Before(hstring const& value)
    {
        _before = value;
    }

    any FBCursors::FromJson(
        hstring const& JsonText 
        )
    {
        any result;
        FBCursors cursors;
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
                    if  (key == L"after")
                    {
                        found++;
                        cursors.After(current.Value().GetString());
                    }
                    else if (key == L"before")
                    {
                        found++;
                        cursors.Before(current.Value().GetString());
                    }
                }

				if(found){
                    result = cursors;
				}
            }
        }
        return result;
    }
}


