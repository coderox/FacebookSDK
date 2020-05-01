#pragma once

#include <winrt/Windows.Foundation.h>

namespace winsdkfb::Graph
{
    struct FBProfilePicture
    {
        static std::shared_ptr<winsdkfb::Graph::FBProfilePicture> FromJson(
            winrt::hstring const& JsonText
        );

        int Height();
        void Height(int const& value);

        bool IsSilhouette();
        void IsSilhouette(bool const& value);

        winrt::hstring Url();
        void Url(winrt::hstring const& value);

        int Width();
        void Width(int const& value);

    private:

        int _height;
        bool _is_silhouette;
        winrt::hstring _url;
        int _width;
    };
}
