#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBObject : public winsdkfb::FBResult
    {
        static FBObject FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

    private:

        winrt::hstring _id;
    };
}
