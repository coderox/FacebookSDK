#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPermission
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Permission() const;
        void Permission(std::wstring const& value);

        std::wstring Status() const;
        void Status(std::wstring const& value);

    private:

        std::wstring _permission;
        std::wstring _status;
    };
}
