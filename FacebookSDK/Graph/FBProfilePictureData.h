#pragma once
#include "FBProfilePicture.h"
#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBProfilePictureData
    {
    	static std::any FromJson(
            std::wstring const& jsonText
        );

        winsdkfb::Graph::FBProfilePicture Data() const;
        void Data(winsdkfb::Graph::FBProfilePicture value);

    private:
        winsdkfb::Graph::FBProfilePicture _data;
    };
}
