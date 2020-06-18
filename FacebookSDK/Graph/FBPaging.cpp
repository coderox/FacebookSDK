#include "Graph/FBPaging.h"

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

    any FBPaging::FromJson(
        hstring const& JsonText 
        )
    {
        any result;
        FBPaging paging;
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
                        auto cursors = FBCursors::FromJson(current.Value().Stringify());
                        if (cursors.has_value()) {
                            found++;
                            paging.Cursors(any_cast<FBCursors>(cursors));
                        }
                    }
                    else if (key == L"next")
                    {
                        found++;
                        paging.Next(current.Value().GetString());
                    }
                    else if (key == L"previous")
                    {
                        found++;
                        paging.Previous(current.Value().GetString());
                    }
                }

				if(found){
					result = paging;
				}
            }
        }
        return result;
    }
}
