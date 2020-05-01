#pragma once

#include <winrt/base.h>

namespace winsdkfb::Graph
{
    struct FBCursors
    {
        static std::shared_ptr<winsdkfb::Graph::FBCursors> FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring After();
        void After(winrt::hstring const& value);

        winrt::hstring Before();
        void Before(winrt::hstring const& value);

    private:

        winrt::hstring _after;
        winrt::hstring _before;
    };
}
