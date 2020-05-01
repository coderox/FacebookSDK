#pragma once
#include "FBProfilePicture.h"

namespace winsdkfb::Graph
{
    struct FBProfilePictureData
    {
        static std::shared_ptr<winsdkfb::Graph::FBProfilePictureData> FromJson(
            winrt::hstring const& JsonText
        );

        std::shared_ptr<winsdkfb::Graph::FBProfilePicture> Data();
        void Data(std::shared_ptr<winsdkfb::Graph::FBProfilePicture> value);

    private:

        std::shared_ptr<winsdkfb::Graph::FBProfilePicture> _data{ nullptr };
    };
}
