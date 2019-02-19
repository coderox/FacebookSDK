#include "pch.h"
#include "FBResult.h"

namespace winrt::winsdkfb::implementation
{
	FBResult::FBResult(Windows::Foundation::IInspectable const& Object)
		: _error(nullptr)
		, _object(nullptr)
	{
		_error = Object.try_as<winrt::winsdkfb::FBError>();
		if (!_error) {
			// Not an error, save as our object
			_object = Object;
		}
	}

	FBResult::~FBResult() {

	}

	bool FBResult::Succeeded()
	{
		return _object != nullptr;
	}

	Windows::Foundation::IInspectable FBResult::Object()
	{
		return _object;
	}

	winsdkfb::FBError FBResult::ErrorInfo()
	{
		return _error;
	}
}
