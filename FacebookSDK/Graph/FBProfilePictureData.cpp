#include "FBProfilePictureData.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    shared_ptr<FBProfilePicture> FBProfilePictureData::Data()
    {
        return _data;
    }
    void FBProfilePictureData::Data(shared_ptr<FBProfilePicture> value)
    {
        _data = value;
    }

    shared_ptr<FBProfilePictureData> FBProfilePictureData::FromJson(
        hstring const& JsonText
    )
    {
        auto result = make_shared<FBProfilePictureData>();
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
                    if (key == L"data")
                    {
                        found++;
                        result->Data(FBProfilePicture::FromJson(current.Value().Stringify()));
                    }
                }

                if (!found) {
                    result = nullptr;
                }
            }
        }
        return result;
    }
}
