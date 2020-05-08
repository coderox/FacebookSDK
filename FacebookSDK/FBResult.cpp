#include "FBResult.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBResult::FBResult() 
	{
		_error.reset();
		_object.reset();
	}

	FBResult::FBResult(std::any&& object) : FBResult()
	{
		try {
			_error = std::any_cast<winsdkfb::FBError>(object);
		}
		catch (...) {
			_object = object;
		}
	}

	FBResult::~FBResult() {
		if (_object.has_value()) {
			_object.reset();
		}
	}

	bool FBResult::Succeeded() {
		return _object.has_value();
	}

	winsdkfb::FBError FBResult::ErrorInfo() {
		return std::any_cast<winsdkfb::FBError>(_error);
	}
}
