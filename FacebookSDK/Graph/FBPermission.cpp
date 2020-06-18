#include "Graph/FBPermission.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBPermission::Permission()
    {
        return _permission;
    }
    void FBPermission::Permission(hstring const& value)
    {
        _permission = value;
    }

    hstring FBPermission::Status()
    {
        return _status;
    }
    void FBPermission::Status(hstring const& value)
    {
        _status = value;
    }

    any FBPermission::FromJson(
        hstring const& JsonText
    )
    {
        any result;
        FBPermission permission;
        int found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(JsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
                JsonObject obj = val.GetObject();
                
                for (auto const& current : obj)
                {
                    winrt::hstring key = current.Key();
                    if (key == L"permission")
                    {
                        found++;
                        permission.Permission(current.Value().GetString());
                    }
                    else if (key == L"status")
                    {
                        found++;
                        permission.Status(current.Value().GetString());
                    }
                }

                if (found) {
                    result = permission;
                }
            }
        }
        return result;
    }
}
