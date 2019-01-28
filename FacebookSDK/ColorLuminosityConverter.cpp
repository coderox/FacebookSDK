#include "pch.h"
#include "ColorLuminosityConverter.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;

namespace winrt::FacebookSDK::implementation
{
	Color ColorLuminosityConverter::ApplyFactor(Color const& color, double factor)
	{
		HlsColor hlsColor;
		hlsColor.RgbValue(color);
		hlsColor.Luminosity(hlsColor.Luminosity() * factor);
		return hlsColor.RgbValue();
	}

	IInspectable ColorLuminosityConverter::Convert(
		IInspectable const& value,
		TypeName const& /*targetType*/,
		IInspectable const & parameter,
		hstring const& /*language*/
	)
	{
		hstring paramString = parameter.as<IPropertyValue>().GetString();
		double factor = stod(wstring(paramString.data()));
		IInspectable result;

		SolidColorBrush scb = value.try_as<SolidColorBrush>();
		if (scb != nullptr)
		{
			result = SolidColorBrush(ApplyFactor(scb.Color(), factor));
		}
		else
		{
			LinearGradientBrush lgb = value.as<LinearGradientBrush>();
			if (lgb != nullptr)
			{
				GradientStopCollection gradientStops;
				for (auto && stop : lgb.GradientStops())
				{
					GradientStop newStop;
					newStop.Color(ApplyFactor(stop.Color(), factor));
					newStop.Offset(stop.Offset());
					gradientStops.Append(newStop);
				}

				result = LinearGradientBrush(gradientStops, 0.0);
			}
		}

		return result;
	}
}