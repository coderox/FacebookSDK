#pragma once

#include <winrt/base.h>

namespace winsdkfb::Graph
{
    struct FBObject
    {
        static std::shared_ptr<FBObject> FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

    private:

        winrt::hstring _id;
    };
}
