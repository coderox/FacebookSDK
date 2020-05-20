#include "Graph/FBProfilePicture.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    int FBProfilePicture::Height()
    {
        return _height;
    }
    void FBProfilePicture::Height(int const& value)
    {
        _height = value;
    }

    bool FBProfilePicture::IsSilhouette()
    {
        return _is_silhouette;
    }
    void FBProfilePicture::IsSilhouette(bool const& value)
    {
        _is_silhouette = value;
    }

    hstring FBProfilePicture::Url()
    {
        return _url;
    }
    void FBProfilePicture::Url(hstring const& value)
    {
        _url = value;
    }

    int FBProfilePicture::Width()
    {
        return _width;
    }
    void FBProfilePicture::Width(int const& value)
    {
        _width = value;
    }

    FBProfilePicture FBProfilePicture::FromJson(
        hstring const& JsonText
    )
    {
        FBProfilePicture result;
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
                    if (key == L"height")
                    {
                        found++;
                        result.Height(static_cast<int>(current.Value().GetNumber()));
                    }
                    else if (key == L"is_silhouette")
                    {
                        found++;
                        result.IsSilhouette(current.Value().GetBoolean());
                    }
                    else if (key == L"url")
                    {
                        found++;
                        result.Url(current.Value().GetString());
                    }
                    else if (key == L"width")
                    {
                        found++;
                        result.Width(static_cast<int>(current.Value().GetNumber()));
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
