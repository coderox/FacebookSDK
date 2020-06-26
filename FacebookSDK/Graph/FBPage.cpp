#include "Graph/FBPage.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    wstring FBPage::Id() const {
        return _id;
    }
	void FBPage::Id(wstring const& value)
    {
        _id = value;
    }

    wstring FBPage::Category() const {
        return _category;
    }
	void FBPage::Category(wstring const& value)
    {
        _category = value;
    }

    int FBPage::CheckIns() const {
        return _checkins;
    }
	void FBPage::CheckIns(int const& value)
    {
        _checkins = value;
    }

    wstring FBPage::Description() const {
        return _description;
    }
	void FBPage::Description(wstring const& value)
    {
        _description = value;
    }

    int FBPage::Likes() const {
        return _likes;
    }
    void FBPage::Likes(int const& value)
    {
        _likes = value;
    }

    wstring FBPage::Link() const {
        return _link;
    }
    void FBPage::Link(wstring const& value)
    {
        _link = value;
    }

    wstring FBPage::Name() const {
        return _name;
    }
	void FBPage::Name(wstring const& value)
    {
        _name = value;
    }

    any FBPage::FromJson(
		wstring const& jsonText
    )
    {
        any result;
        FBPage page;
        auto found = 0;
        JsonValue val{ nullptr };

        if (JsonValue::TryParse(jsonText, val))
        {
            if (val.ValueType() == JsonValueType::Object)
            {
	            auto obj = val.GetObject();
                for (auto&& current : obj)
                {
	                auto key = current.Key();
                    if (key == L"id")
                    {
                        found++;
                        page._id = current.Value().GetString();
                    }
                    else if (key == L"category")
                    {
                        found++;
                        page._category = current.Value().GetString();
                    }
                    else if (key == L"checkins")
                    {
                        found++;
						page.CheckIns(static_cast<int>(current.Value().GetNumber()));
                    }
                    else if (key == L"description")
                    {
                        found++;
                        page._description = current.Value().GetString();
                    }
                    else if (key == L"likes")
                    {
                        found++;
                        page._likes = static_cast<int>(current.Value().GetNumber());
                    }
                    else if (key == L"link")
                    {
                        found++;
                        page._link = current.Value().GetString();
                    }
                    else if (key == L"name")
                    {
                        found++;
                        page._name = current.Value().GetString();
                    }
                }

                if (found) {
                    result = page;
                }
            }
        }
        return result;
    }
}
