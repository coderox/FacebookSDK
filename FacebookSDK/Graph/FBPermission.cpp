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
    wstring FBPermission::Permission() const {
        return _permission;
    }
	void FBPermission::Permission(wstring const& value)
    {
        _permission = value;
    }

    wstring FBPermission::Status() const {
        return _status;
    }
	void FBPermission::Status(wstring const& value)
    {
        _status = value;
    }

    any FBPermission::FromJson(
		wstring const& jsonText
    )
    {
        any result;
        FBPermission permission;
        int found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(jsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
                JsonObject obj = val.GetObject();
                
                for (auto const& current : obj)
                {
                    std::wstring key = current.Key().c_str();
                    if (key == L"permission")
                    {
                        found++;
                        permission._permission = current.Value().GetString();
                    }
                    else if (key == L"status")
                    {
                        found++;
                        permission._status = current.Value().GetString();
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
