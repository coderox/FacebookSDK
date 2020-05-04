#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBCursors : public winsdkfb::FBResult
    {
        static winsdkfb::Graph::FBCursors FromJson(
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
