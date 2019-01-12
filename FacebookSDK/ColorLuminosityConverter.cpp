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
	IInspectable ColorLuminosityConverter::Convert(
		IInspectable const& value,
		TypeName const& targetType,
		IInspectable const & parameter,
		hstring const& language
	)
	{
		hstring paramString = parameter.as<IPropertyValue>().GetString();
		double factor = stod(wstring(paramString.data()));
		IInspectable result = nullptr;
		HlsColor hlsColor;

		SolidColorBrush scb = value.as<SolidColorBrush>();
		if (scb != nullptr)
		{
			Color brushColor = scb.Color();
			hlsColor.RgbValue(brushColor);
			hlsColor.Luminosity(hlsColor.Luminosity() * factor);
			Color newColor = hlsColor.RgbValue();
			result = SolidColorBrush(newColor);
		}
		else
		{
			LinearGradientBrush lgb = value.as<LinearGradientBrush>();
			if (lgb != nullptr)
			{
				GradientStopCollection gradientStops;
				for (auto && stop : lgb.GradientStops())
				{
					//GradientStop^ stop = iter->Current;
					GradientStop newStop;

					hlsColor.RgbValue(stop.Color());
					hlsColor.Luminosity(hlsColor.Luminosity() * factor);
					newStop.Color(hlsColor.RgbValue());
					newStop.Offset(stop.Offset());
					gradientStops.Append(newStop);
				}

				result = LinearGradientBrush(gradientStops, 0.0);
			}
		}

		return result;
	}
}