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

    any FBProfilePictureData::FromJson(
        hstring const& JsonText
    )
    {
        any result;
        FBProfilePictureData profilePictureData;
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
                        auto profilePicture = FBProfilePicture::FromJson(current.Value().Stringify());
                        if (profilePicture.has_value()) {
                            found++;
                            profilePictureData.Data(any_cast<FBProfilePicture>(profilePicture));
                        }
                    }
                }

                if (found) {
                    result = profilePictureData;
                }
            }
        }
        return result;
    }
}
