#pragma once

#include "FBPage.h"
#include "FBProfilePictureData.h"
#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBUser
    {
        static std::any FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

        winrt::hstring FirstName();
        void FirstName(winrt::hstring const& value);

        winrt::hstring Gender();
        void Gender(winrt::hstring const& value);

        winrt::hstring LastName();
        void LastName(winrt::hstring const& value);

        winrt::hstring Email();
        void Email(winrt::hstring const& value);

        winrt::hstring Link();
        void Link(winrt::hstring const& value);

        winrt::hstring Locale();
        void Locale(winrt::hstring const& value);

        winsdkfb::Graph::FBPage Location();
        void Location(winsdkfb::Graph::FBPage value);

        winrt::hstring Name();
        void Name(winrt::hstring const& value);

        winsdkfb::Graph::FBProfilePictureData Picture();
        void Picture(winsdkfb::Graph::FBProfilePictureData value);

        int Timezone();
        void Timezone(int const& value);

        winrt::hstring UpdatedTime();
        void UpdatedTime(winrt::hstring const& value);

        bool Verified();
        void Verified(bool const& value);

    private:

        winrt::hstring _id;
        winrt::hstring _first_name;
        winrt::hstring _gender;
        winrt::hstring _last_name;
        winrt::hstring _email;
        winrt::hstring _link;
        winrt::hstring _locale;
        winsdkfb::Graph::FBPage _location;
        winrt::hstring _name;
        winsdkfb::Graph::FBProfilePictureData _picture;
        int __timezone;
        winrt::hstring _updated_time;
        bool _verified;
    };
}
