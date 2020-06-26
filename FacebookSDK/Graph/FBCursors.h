#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBCursors 
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring After() const;
        void After(std::wstring const& value);

        std::wstring Before() const;
        void Before(std::wstring const& value);

    private:

        std::wstring _after;
        std::wstring _before;
    };
}
