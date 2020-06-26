#pragma once

#include "FBCursors.h"

namespace winsdkfb::Graph
{
    struct FBPaging
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        winsdkfb::Graph::FBCursors Cursors() const;
        void Cursors(winsdkfb::Graph::FBCursors value);

        std::wstring Next() const;
        void Next(std::wstring const& value);

        std::wstring Previous() const;
        void Previous(std::wstring const& value);

    private:

        winsdkfb::Graph::FBCursors _cursors;
        std::wstring _next;
        std::wstring _previous;
    };
}
