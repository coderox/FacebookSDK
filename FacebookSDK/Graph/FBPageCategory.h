#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPageCategory
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Id() const;
        void Id(std::wstring const& value);

        std::wstring Name() const;
        void Name(std::wstring const& value);

    private:

        std::wstring _id;
        std::wstring _name;
    };
}
