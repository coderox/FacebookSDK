#include "FBResult.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBResult::FBResult() 
	{
		_object.reset();
	}

	FBResult::FBResult(std::any&& object) : FBResult()
	{
		try {
			if (object.type() == typeid(winsdkfb::FBError)) {
				_error = std::any_cast<winsdkfb::FBError>(object);
			}
			else {
				_object = object;
			}
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

	bool FBResult::Succeeded() const {
		return _object.has_value();
	}

	winsdkfb::FBError FBResult::ErrorInfo() {
		return std::any_cast<winsdkfb::FBError>(_error);
	}
}
