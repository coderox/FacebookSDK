#include "Graph/FBPageCategory.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    wstring FBPageCategory::Id() const {
        return _id;
    }
	void FBPageCategory::Id(wstring const& value)
    {
        _id = value;
    }

    wstring FBPageCategory::Name() const {
        return _name;
    }
	void FBPageCategory::Name(wstring const& value)
    {
        _name = value;
    }

    any FBPageCategory::FromJson(
		wstring const& jsonText 
        )
    {
        any result;
        FBPageCategory pageCategory;
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
                        pageCategory._id = current.Value().GetString();
                    }
                    else if (key == L"name")
                    {
                        found++;
                        pageCategory._name = current.Value().GetString();
                    }
                }

				if(found){
					result = pageCategory;
				}
            }
        }
        return result;
    }
}



