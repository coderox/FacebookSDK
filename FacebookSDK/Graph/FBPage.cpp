#include "FBPage.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBPage::Id()
    {
        return _id;
    }
    void FBPage::Id(hstring const& value)
    {
        _id = value;
    }

    hstring FBPage::Category()
    {
        return _category;
    }
    void FBPage::Category(hstring const& value)
    {
        _category = value;
    }

    int FBPage::Checkins()
    {
        return _checkins;
    }
    void FBPage::Checkins(int const& value)
    {
        _checkins = value;
    }

    hstring FBPage::Description()
    {
        return _description;
    }
    void FBPage::Description(hstring const& value)
    {
        _description = value;
    }

    int FBPage::Likes()
    {
        return _likes;
    }
    void FBPage::Likes(int const& value)
    {
        _likes = value;
    }

    hstring FBPage::Link()
    {
        return _link;
    }
    void FBPage::Link(hstring const& value)
    {
        _link = value;
    }

    hstring FBPage::Name()
    {
        return _name;
    }
    void FBPage::Name(hstring const& value)
    {
        _name = value;
    }

    FBPage FBPage::FromJson(
        hstring const& JsonText
    )
    {
        FBPage result;
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
                    if (key == L"id")
                    {
                        found++;
                        result.Id(current.Value().GetString());
                    }
                    else if (key == L"category")
                    {
                        found++;
                        result.Category(current.Value().GetString());
                    }
                    else if (key == L"checkins")
                    {
                        found++;
                        result.Checkins(static_cast<int>(current.Value().GetNumber()));
                    }
                    else if (key == L"description")
                    {
                        found++;
                        result.Description(current.Value().GetString());
                    }
                    else if (key == L"likes")
                    {
                        found++;
                        result.Likes(static_cast<int>(current.Value().GetNumber()));
                    }
                    else if (key == L"link")
                    {
                        found++;
                        result.Link(current.Value().GetString());
                    }
                    else if (key == L"name")
                    {
                        found++;
                        result.Name(current.Value().GetString());
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
