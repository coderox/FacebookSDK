#include "FBCursors.h"

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

    FBCursors FBCursors::FromJson(
        hstring const& JsonText 
        )
    {
        FBCursors result;
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
                        result.After(current.Value().GetString());
                    }
                    else if (key == L"before")
                    {
                        found++;
                        result.Before(current.Value().GetString());
                    }
                }

				//if(found){
    //                result._succeeded = true;
				//}
            }
        }
        return result;
    }
}


