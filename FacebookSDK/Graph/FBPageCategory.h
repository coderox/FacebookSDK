#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPageCategory
    {
        static std::any FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

        winrt::hstring Name();
        void Name(winrt::hstring const& value);

    private:

        winrt::hstring _id;
        winrt::hstring _name;
    };
}
