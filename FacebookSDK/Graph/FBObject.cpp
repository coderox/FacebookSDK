#include "FBObject.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBObject::Id()
    {
        return _id;
    }
    void FBObject::Id(hstring const& value)
    {
        _id = value;
    }

    shared_ptr<FBObject> FBObject::FromJson(
        hstring const& JsonText 
        )
    {
        auto result = make_shared<FBObject>();
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
                        result->Id(current.Value().GetString());
                    }
                }

				if(!found){
					result = nullptr;
				}
            }
        }
        return result;
    }
}
