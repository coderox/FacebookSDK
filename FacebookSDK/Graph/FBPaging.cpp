#include "FBPaging.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    FBCursors FBPaging::Cursors()
    {
        return _cursors;
    }
    void FBPaging::Cursors(FBCursors value)
    {
        _cursors = value;
    }

    hstring FBPaging::Next()
    {
        return _next;
    }
    void FBPaging::Next(hstring const& value)
    {
        _next = value;
    }

    hstring FBPaging::Previous()
    {
        return _previous;
    }
    void FBPaging::Previous(hstring const& value)
    {
        _previous = value;
    }

    FBPaging FBPaging::FromJson(
        hstring const& JsonText 
        )
    {
        FBPaging result;
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
                    if  (key == L"cursors")
                    {
                        found++;
                        result.Cursors(FBCursors::FromJson(current.Value().Stringify()));
                    }
                    else if (key == L"next")
                    {
                        found++;
                        result.Next(current.Value().GetString());
                    }
                    else if (key == L"previous")
                    {
                        found++;
                        result.Previous(current.Value().GetString());
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
