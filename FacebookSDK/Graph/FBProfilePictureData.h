#pragma once
#include "FBProfilePicture.h"
#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBProfilePictureData : public winsdkfb::FBResult
    {
        static winsdkfb::Graph::FBProfilePictureData FromJson(
            winrt::hstring const& JsonText
        );

        winsdkfb::Graph::FBProfilePicture Data();
        void Data(winsdkfb::Graph::FBProfilePicture value);

    private:

        winsdkfb::Graph::FBProfilePicture _data;
    };
}
