#include "pch.h"
#include "FacebookResult.h"

namespace winrt::FacebookSDK::implementation
{
    FacebookResult::FacebookResult(Windows::Foundation::IInspectable const& Object)
		: _error(nullptr)
		, _object(nullptr)
    {
		_error = Object.try_as<winrt::FacebookSDK::FacebookError>();
		if (!_error) {
			// Not an error, save as our object
			_object = Object;
		}
	}

	FacebookResult::~FacebookResult() {

	}

    bool FacebookResult::Succeeded()
    {
		return _object != nullptr;
    }

    Windows::Foundation::IInspectable FacebookResult::Object()
    {
		return _object;
    }

    FacebookSDK::FacebookError FacebookResult::ErrorInfo()
    {
		return _error;
	}
}
