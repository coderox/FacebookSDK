#include "FBPermission.h"

#include <winrt/Windows.Data.Json.h>

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

    FBResult FBPermission::FromJson(
        hstring const& JsonText
    )
    {
        FBPermission result;
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
                    if (key == L"permission")
                    {
                        found++;
                        result.Permission(current.Value().GetString());
                    }
                    else if (key == L"status")
                    {
                        found++;
                        result.Status(current.Value().GetString());
                    }
                }

                if (found) {
                    result._succeeded = true;
                }
            }
        }
        return result;
    }
}
