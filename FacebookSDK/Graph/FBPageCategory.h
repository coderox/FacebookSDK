#pragma once

#include <winrt/base.h>

namespace winsdkfb::Graph
{
    struct FBPageCategory
    {
        static std::shared_ptr<winsdkfb::Graph::FBPageCategory> FromJson(
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
