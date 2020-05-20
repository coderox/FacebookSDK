#include "Graph/FBProfilePictureData.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    FBProfilePicture FBProfilePictureData::Data()
    {
        return _data;
    }
    void FBProfilePictureData::Data(FBProfilePicture value)
    {
        _data = value;
    }

    FBProfilePictureData FBProfilePictureData::FromJson(
        hstring const& JsonText
    )
    {
        FBProfilePictureData result;
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
                        result.Data(FBProfilePicture::FromJson(current.Value().Stringify()));
                    }
                }

                //if (found) {
                //    result._succeeded = true;
                //}
            }
        }
        return result;
    }
}
