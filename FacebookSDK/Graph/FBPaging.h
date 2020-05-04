#pragma once

#include "FBCursors.h"

namespace winsdkfb::Graph
{
    struct FBPaging : public winsdkfb::FBResult
    {
        static winsdkfb::Graph::FBPaging FromJson(
            winrt::hstring const& JsonText
        );

        winsdkfb::Graph::FBCursors Cursors();
        void Cursors(winsdkfb::Graph::FBCursors value);

        winrt::hstring Next();
        void Next(winrt::hstring const& value);

        winrt::hstring Previous();
        void Previous(winrt::hstring const& value);

    private:

        winsdkfb::Graph::FBCursors _cursors;
        winrt::hstring _next;
        winrt::hstring _previous;
    };
}
