#include "FBResult.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBResult::FBResult(FBError error)
		: _error(error)
		, _succeeded(false)
	{
		
	}

	bool FBResult::Succeeded()
	{
		return _succeeded;
	}

	FBError FBResult::ErrorInfo()
	{
		return _error;
	}
}
