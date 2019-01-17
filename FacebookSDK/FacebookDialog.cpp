#include "pch.h"
#include "FacebookDialog.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::FacebookSDK::implementation
{
    FacebookDialog::FacebookDialog()
    {
        InitializeComponent();
    }

    int32_t FacebookDialog::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void FacebookDialog::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void FacebookDialog::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
