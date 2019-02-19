#pragma once

#include "ScaleConverter.g.h"

namespace winrt::winsdkfb::implementation
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

namespace winrt::winsdkfb::factory_implementation
{
	struct ScaleConverter : ScaleConverterT<ScaleConverter, implementation::ScaleConverter>
	{
	};
}
