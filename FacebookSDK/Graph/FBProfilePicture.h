#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBProfilePicture
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        int Height() const;
        void Height(int const& value);

        bool IsSilhouette() const;
        void IsSilhouette(bool const& value);

        std::wstring Url() const;
        void Url(std::wstring const& value);

        int Width() const;
        void Width(int const& value);

    private:

        int _height = 0;
        bool _is_silhouette = false;
        std::wstring _url;
        int _width = 0;
    };
}
