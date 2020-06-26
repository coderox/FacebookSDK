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
    int FBProfilePicture::Height() const {
        return _height;
    }
    void FBProfilePicture::Height(int const& value)
    {
        _height = value;
    }

    bool FBProfilePicture::IsSilhouette() const {
        return _is_silhouette;
    }
    void FBProfilePicture::IsSilhouette(bool const& value)
    {
        _is_silhouette = value;
    }

    wstring FBProfilePicture::Url() const {
        return _url;
    }
	void FBProfilePicture::Url(wstring const& value)
    {
        _url = value;
    }

    int FBProfilePicture::Width() const {
        return _width;
    }
    void FBProfilePicture::Width(int const& value)
    {
        _width = value;
    }

    any FBProfilePicture::FromJson(
		wstring const& jsonText
    )
    {
        any result;
        FBProfilePicture profilePicture;
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
                    if (key == L"height")
                    {
                        found++;
                        profilePicture._height = static_cast<int>(current.Value().GetNumber());
                    }
                    else if (key == L"is_silhouette")
                    {
                        found++;
                        profilePicture._is_silhouette = current.Value().GetBoolean();
                    }
                    else if (key == L"url")
                    {
                        found++;
                        profilePicture._url = current.Value().GetString();
                    }
                    else if (key == L"width")
                    {
                        found++;
                        profilePicture._width = static_cast<int>(current.Value().GetNumber());
                    }
                }

                if (found) {
                    result = profilePicture;
                }
            }
        }
        return result;
    }
}
