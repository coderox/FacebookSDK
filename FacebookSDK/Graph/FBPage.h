#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPage : public winsdkfb::FBResult
    {
        static winsdkfb::Graph::FBPage FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

        winrt::hstring Category();
        void Category(winrt::hstring const& value);

        int Checkins();
        void Checkins(int const& value);

        winrt::hstring Description();
        void Description(winrt::hstring const& value);

        int Likes();
        void Likes(int const& value);

        winrt::hstring Link();
        void Link(winrt::hstring const& value);

        winrt::hstring Name();
        void Name(winrt::hstring const& value);

    private:

        winrt::hstring _id;
        winrt::hstring _category;
        int _checkins;
        winrt::hstring _description;
        int _likes;
        winrt::hstring _link;
        winrt::hstring _name;
    };
}
