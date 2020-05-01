#pragma once

#include "FBCursors.h"

namespace winsdkfb::Graph
{
    struct FBPaging
    {
        static std::shared_ptr<winsdkfb::Graph::FBPaging> FromJson(
            winrt::hstring const& JsonText
        );

        std::shared_ptr<winsdkfb::Graph::FBCursors> Cursors();
        void Cursors(std::shared_ptr<winsdkfb::Graph::FBCursors> value);

        winrt::hstring Next();
        void Next(winrt::hstring const& value);

        winrt::hstring Previous();
        void Previous(winrt::hstring const& value);

    private:

        std::shared_ptr<winsdkfb::Graph::FBCursors> _cursors{ nullptr };
        winrt::hstring _next;
        winrt::hstring _previous;
    };
}
