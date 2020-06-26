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
    FBCursors FBPaging::Cursors() const {
        return _cursors;
    }
    void FBPaging::Cursors(FBCursors value)
    {
        _cursors = value;
    }

    wstring FBPaging::Next() const {
        return _next;
    }
	void FBPaging::Next(wstring const& value)
    {
        _next = value;
    }

    wstring FBPaging::Previous() const {
        return _previous;
    }
	void FBPaging::Previous(wstring const& value)
    {
        _previous = value;
    }

    any FBPaging::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBPaging paging;
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
                    if  (key == L"cursors")
                    {
                        auto cursors = FBCursors::FromJson(current.Value().Stringify().c_str());
                        if (cursors.has_value()) {
                            found++;
                            paging._cursors = any_cast<FBCursors>(cursors);
                        }
                    }
                    else if (key == L"next")
                    {
                        found++;
                        paging._next = current.Value().GetString();
                    }
                    else if (key == L"previous")
                    {
                        found++;
                        paging._previous = current.Value().GetString();
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
