#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPermission
    {
        static std::any FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Permission();
        void Permission(winrt::hstring const& value);

        winrt::hstring Status();
        void Status(winrt::hstring const& value);

    private:

        winrt::hstring _permission;
        winrt::hstring _status;
    };
}
