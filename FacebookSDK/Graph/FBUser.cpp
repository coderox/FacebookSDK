#include "Graph/FBUser.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winsdkfb::Graph
{
    wstring FBUser::Id() const {
        return _id;
    }
	void FBUser::Id(wstring const& value)
    {
        _id = value;
    }

    wstring FBUser::FirstName() const {
        return _first_name;
    }
	void FBUser::FirstName(wstring const& value)
    {
        _first_name = value;
    }

    wstring FBUser::Gender() const {
        return _gender;
    }
	void FBUser::Gender(wstring const& value)
    {
        _gender = value;
    }

    wstring FBUser::LastName() const {
        return _last_name;
    }
	void FBUser::LastName(wstring const& value)
    {
        _last_name = value;
    }

    wstring FBUser::Email() const {
        return _email;
    }
	void FBUser::Email(wstring const& value)
    {
        _email = value;
    }

    wstring FBUser::Link() const {
        return _link;
    }
	void FBUser::Link(wstring const& value)
    {
        _link = value;
    }

    wstring FBUser::Locale() const {
        return _locale;
    }
	void FBUser::Locale(wstring const& value)
    {
        _locale = value;
    }

    FBPage FBUser::Location() const {
        return _location;
    }
    void FBUser::Location(FBPage value)
    {
        _location = value;
    }

    wstring FBUser::Name() const {
        return _name;
    }
	void FBUser::Name(wstring const& value)
    {
        _name = value;
    }

    winsdkfb::Graph::FBProfilePictureData FBUser::Picture() const {
        return _picture;
    }
    void FBUser::Picture(FBProfilePictureData value)
    {
        _picture = value;
    }

    int FBUser::Timezone() const {
        return __timezone;
    }
    void FBUser::Timezone(int const& value)
    {
        __timezone = value;
    }

    wstring FBUser::UpdatedTime() const {
        return _updated_time;
    }
	void FBUser::UpdatedTime(wstring const& value)
    {
        _updated_time = value;
    }

    bool FBUser::Verified() const {
        return _verified;
    }
    void FBUser::Verified(bool const& value)
    {
        _verified = value;
    }

    any FBUser::FromJson(
		wstring const& jsonText
    )
    {
        any result;
        FBUser user;
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
                    if (key == L"id")
                    {
                        found++;
                        user._id = current.Value().GetString();
                    }
                    else if (key == L"first_name")
                    {
                        found++;
                        user._first_name = current.Value().GetString();
                    }
                    else if (key == L"gender")
                    {
                        found++;
                        user._gender = current.Value().GetString();
                    }
                    else if (key == L"last_name")
                    {
                        found++;
                        user._last_name = current.Value().GetString();
                    }
                    else if (key == L"email")
                    {
                        found++;
                        user._email = current.Value().GetString();
                    }
                    else if (key == L"link")
                    {
                        found++;
                        user._link = current.Value().GetString();
                    }
                    else if (key == L"locale")
                    {
                        found++;
                        user._locale = current.Value().GetString();
                    }
                    else if (key == L"location")
                    {
                        auto page = FBPage::FromJson(current.Value().Stringify().c_str());
                        if (page.has_value()) {
                            found++;
                            user._location = any_cast<winsdkfb::Graph::FBPage>(page);
                        }
                    }
                    else if (key == L"name")
                    {
                        found++;
                        user._name = current.Value().GetString();
                    }
                    else if (key == L"picture")
                    {
                        auto profilePictureData = FBProfilePictureData::FromJson(current.Value().Stringify().c_str());
                        if (profilePictureData.has_value()) {
                            found++;
                            user._picture = any_cast<FBProfilePictureData>(profilePictureData);
                        }
                    }
                    else if (key == L"timezone")
                    {
                        found++;
                        user.__timezone = static_cast<int>(current.Value().GetNumber());
                    }
                    else if (key == L"updated_time")
                    {
                        found++;
                        user._updated_time = current.Value().GetString();
                    }
                    else if (key == L"verified")
                    {
                        found++;
                        user._verified = current.Value().GetBoolean();
                    }
                }

                if (found) {
                    result = user;
                }
            }
        }
        return result;
    }
}
