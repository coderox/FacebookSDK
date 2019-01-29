#pragma once

#include "ColorLuminosityConverter.g.h"
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt::FacebookSDK::implementation
{
	struct ColorLuminosityConverter : ColorLuminosityConverterT<ColorLuminosityConverter>
	{
		ColorLuminosityConverter() { }

		Windows::UI::Color ApplyFactor(Windows::UI::Color const& color, double factor);

		virtual Windows::Foundation::IInspectable Convert(
			Windows::Foundation::IInspectable const& value,
			Windows::UI::Xaml::Interop::TypeName const& targetType,
			Windows::Foundation::IInspectable const& parameter,
			hstring const& language);

		virtual Windows::Foundation::IInspectable ConvertBack(
			Windows::Foundation::IInspectable const& /*value*/,
			Windows::UI::Xaml::Interop::TypeName const& /*targetType*/,
			Windows::Foundation::IInspectable const& /*parameter*/,
			hstring const& /*language*/)
		{
			throw hresult_not_implemented();
		}
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct ColorLuminosityConverter : ColorLuminosityConverterT<ColorLuminosityConverter, implementation::ColorLuminosityConverter>
	{
	};
}