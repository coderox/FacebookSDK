#include "FBUser.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    hstring FBUser::Id()
    {
        return _id;
    }
    void FBUser::Id(hstring const& value)
    {
        _id = value;
    }

    hstring FBUser::FirstName()
    {
        return _first_name;
    }
    void FBUser::FirstName(hstring const& value)
    {
        _first_name = value;
    }

    hstring FBUser::Gender()
    {
        return _gender;
    }
    void FBUser::Gender(hstring const& value)
    {
        _gender = value;
    }

    hstring FBUser::LastName()
    {
        return _last_name;
    }
    void FBUser::LastName(hstring const& value)
    {
        _last_name = value;
    }

    hstring FBUser::Email()
    {
        return _email;
    }
    void FBUser::Email(hstring const& value)
    {
        _email = value;
    }

    hstring FBUser::Link()
    {
        return _link;
    }
    void FBUser::Link(hstring const& value)
    {
        _link = value;
    }

    hstring FBUser::Locale()
    {
        return _locale;
    }
    void FBUser::Locale(hstring const& value)
    {
        _locale = value;
    }

    shared_ptr<winsdkfb::Graph::FBPage> FBUser::Location()
    {
        return _location;
    }
    void FBUser::Location(shared_ptr<winsdkfb::Graph::FBPage> value)
    {
        _location = value;
    }

    hstring FBUser::Name()
    {
        return _name;
    }
    void FBUser::Name(hstring const& value)
    {
        _name = value;
    }

    shared_ptr<winsdkfb::Graph::FBProfilePictureData> FBUser::Picture()
    {
        return _picture;
    }
    void FBUser::Picture(shared_ptr<winsdkfb::Graph::FBProfilePictureData> value)
    {
        _picture = value;
    }

    int FBUser::Timezone()
    {
        return __timezone;
    }
    void FBUser::Timezone(int const& value)
    {
        __timezone = value;
    }

    hstring FBUser::UpdatedTime()
    {
        return _updated_time;
    }
    void FBUser::UpdatedTime(hstring const& value)
    {
        _updated_time = value;
    }

    bool FBUser::Verified()
    {
        return _verified;
    }
    void FBUser::Verified(bool const& value)
    {
        _verified = value;
    }

    shared_ptr<FBUser> FBUser::FromJson(
        hstring const& JsonText
    )
    {
        auto result = make_shared<FBUser>();
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
                        result->Id(current.Value().GetString());
                    }
                    else if (key == L"first_name")
                    {
                        found++;
                        result->FirstName(current.Value().GetString());
                    }
                    else if (key == L"gender")
                    {
                        found++;
                        result->Gender(current.Value().GetString());
                    }
                    else if (key == L"last_name")
                    {
                        found++;
                        result->LastName(current.Value().GetString());
                    }
                    else if (key == L"email")
                    {
                        found++;
                        result->Email(current.Value().GetString());
                    }
                    else if (key == L"link")
                    {
                        found++;
                        result->Link(current.Value().GetString());
                    }
                    else if (key == L"locale")
                    {
                        found++;
                        result->Locale(current.Value().GetString());
                    }
                    else if (key == L"location")
                    {
                        found++;
                        result->Location(FBPage::FromJson(current.Value().Stringify()));
                    }
                    else if (key == L"name")
                    {
                        found++;
                        result->Name(current.Value().GetString());
                    }
                    else if (key == L"picture")
                    {
                        found++;
                        result->Picture(FBProfilePictureData::FromJson(current.Value().Stringify()));
                    }
                    else if (key == L"timezone")
                    {
                        found++;
                        result->Timezone(static_cast<int>(current.Value().GetNumber()));
                    }
                    else if (key == L"updated_time")
                    {
                        found++;
                        result->UpdatedTime(current.Value().GetString());
                    }
                    else if (key == L"verified")
                    {
                        found++;
                        result->Verified(current.Value().GetBoolean());
                    }
                }

                if (!found) {
                    result = nullptr;
                }
            }
        }
        result->_succeeded = true;
        return result;
    }
}
