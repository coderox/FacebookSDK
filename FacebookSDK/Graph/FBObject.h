#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBObject
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Id() const;
        void Id(std::wstring const& value);

    private:

        std::wstring _id;
    };
}
