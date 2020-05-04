#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPageCategory : public winsdkfb::FBResult
    {
        static winsdkfb::Graph::FBPageCategory FromJson(
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
