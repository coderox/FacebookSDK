#pragma once

#include "HlsColor.g.h"

namespace winrt::winsdkfb::implementation
{
	struct HlsColor : HlsColorT<HlsColor>
	{

	public:
		HlsColor() {}
		Windows::UI::Color RgbValue();
		void RgbValue(Windows::UI::Color const& value);

		double Luminosity();
		void Luminosity(double value);

	private:
		double TransformColor(
			double t1,
			double t2,
			double t3
		);

		double _A;
		double _H;
		double _L;
		double _S;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct HlsColor : HlsColorT<HlsColor, implementation::HlsColor>
	{
	};
}
