#include "FBResult.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBResult::FBResult(std::any object)
		: _object(nullptr)
		, _error(nullptr)
	{
		_error = std::any_cast<winsdkfb::FBError>(&object);
		if (!_error) {
			// Not an error, save as our object
			_object = object;
		}
	}

	FBResult::~FBResult() {
		if (_error) {
			delete(_error);
		}
		if (_object.has_value()) {
			_object.reset();
		}
	}

	template<typename T>
	T FBResult::Object()
	{
		return std::any_cast<T>(&_object);
	}

	bool FBResult::Succeeded() {
		return _object.has_value();
	}

	winsdkfb::FBError FBResult::ErrorInfo() {
		return *_error;
	}
}
