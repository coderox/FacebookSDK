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
    FBProfilePicture FBProfilePictureData::Data() const {
        return _data;
    }
    void FBProfilePictureData::Data(FBProfilePicture value)
    {
        _data = value;
    }

    any FBProfilePictureData::FromJson(
		wstring const& jsonText
    )
    {
        any result;
        FBProfilePictureData profilePictureData;
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
                    if (key == L"data")
                    {
                        auto profilePicture = FBProfilePicture::FromJson(current.Value().Stringify().c_str());
                        if (profilePicture.has_value()) {
                            found++;
                            profilePictureData._data = any_cast<FBProfilePicture>(profilePicture);
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
