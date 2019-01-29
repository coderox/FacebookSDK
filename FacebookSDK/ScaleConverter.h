#pragma once

#include "ScaleConverter.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct ScaleConverter : ScaleConverterT<ScaleConverter>
	{
		ScaleConverter() { }

		Windows::Foundation::IInspectable Convert(
			Windows::Foundation::IInspectable const& value,
			Windows::UI::Xaml::Interop::TypeName const& targetType,
			Windows::Foundation::IInspectable const& parameter,
			hstring const& language);

		Windows::Foundation::IInspectable ConvertBack(
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
	struct ScaleConverter : ScaleConverterT<ScaleConverter, implementation::ScaleConverter>
	{
	};
}
