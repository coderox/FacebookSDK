#pragma once

#include <winrt/Windows.Foundation.h>
#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPermission : public FBResult
    {
        static std::shared_ptr<winsdkfb::FBResult> FromJson(
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
