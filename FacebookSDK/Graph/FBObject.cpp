#include "Graph/FBObject.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    wstring FBObject::Id() const {
        return _id;
    }
	void FBObject::Id(wstring const& value)
    {
        _id = value;
    }

    any FBObject::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBObject object;
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
                    if  (key == L"id")
                    {
                        found++;
                        object._id = current.Value().GetString();
                    }
                }

				if(found){
					result = object;
				}
            }
        }
        return result;
    }
}
