#pragma once

#include "FBPage.h"
#include "FBProfilePictureData.h"
#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBUser
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Id() const;
        void Id(std::wstring const& value);

        std::wstring FirstName() const;
        void FirstName(std::wstring const& value);

        std::wstring Gender() const;
        void Gender(std::wstring const& value);

        std::wstring LastName() const;
        void LastName(std::wstring const& value);

        std::wstring Email() const;
        void Email(std::wstring const& value);

        std::wstring Link() const;
        void Link(std::wstring const& value);

        std::wstring Locale() const;
        void Locale(std::wstring const& value);

        FBPage Location() const;
        void Location(FBPage value);

        std::wstring Name() const;
        void Name(std::wstring const& value);

        FBProfilePictureData Picture() const;
        void Picture(FBProfilePictureData value);

        int Timezone() const;
        void Timezone(int const& value);

        std::wstring UpdatedTime() const;
        void UpdatedTime(std::wstring const& value);

        bool Verified() const;
        void Verified(bool const& value);

    private:
    	std::wstring _id;
        std::wstring _first_name;
        std::wstring _gender;
        std::wstring _last_name;
        std::wstring _email;
        std::wstring _link;
        std::wstring _locale;
        FBPage _location;
        std::wstring _name;
        FBProfilePictureData _picture;
        int __timezone = 0;
        std::wstring _updated_time;
        bool _verified = false;
    };
}
