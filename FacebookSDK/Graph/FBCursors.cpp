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
    wstring FBCursors::After() const {
        return _after;
    }
	void FBCursors::After(wstring const& value)
    {
        _after = value;
    }

    wstring FBCursors::Before() const {
        return _before;
    }
	void FBCursors::Before(wstring const& value)
    {
        _before = value;
    }

    any FBCursors::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBCursors cursors;
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
                    if  (key == L"after")
                    {
                        found++;
						cursors.After(current.Value().GetString().c_str());
                    }
                    else if (key == L"before")
                    {
                        found++;
						cursors.Before(current.Value().GetString().c_str());
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


