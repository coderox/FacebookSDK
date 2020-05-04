#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBGroup : public winsdkfb::FBResult
    {
        winsdkfb::Graph::FBGroup FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

        winrt::hstring Description();
        void Description(winrt::hstring const& value);

        winrt::hstring Email();
        void Email(winrt::hstring const& value);

        winrt::hstring Icon();
        void Icon(winrt::hstring const& value);

        winrt::hstring Link();
        void Link(winrt::hstring const& value);

        winrt::hstring Name();
        void Name(winrt::hstring const& value);

        winrt::hstring Privacy();
        void Privacy(winrt::hstring const& value);

    private:

        winrt::hstring _id;
        winrt::hstring _description;
        winrt::hstring _email;
        winrt::hstring _icon;
        winrt::hstring _link;
        winrt::hstring _name;
        winrt::hstring _privacy;
    };
}
