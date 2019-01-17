//
// Declaration of the FacebookDialog class.
//

#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "FacebookDialog.g.h"

namespace winrt::FacebookSDK::implementation
{
    struct FacebookDialog : FacebookDialogT<FacebookDialog>
    {
        FacebookDialog();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::FacebookSDK::factory_implementation
{
    struct FacebookDialog : FacebookDialogT<FacebookDialog, implementation::FacebookDialog>
    {
    };
}
