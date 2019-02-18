#include "pch.h"
#include "ScaleConverter.h"
#include <string>

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::winsdkfb::implementation
{
	IInspectable ScaleConverter::Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& /*language*/)
	{
		if (targetType != xaml_typename<double>())
		{
			throw hresult_invalid_argument();
		}

		double numValue = unbox_value<double>(value);
		auto paramString = unbox_value<hstring>(parameter);
		double numParam = stod(wstring(paramString.c_str()));
		return box_value(numValue * numParam);
	}
}
