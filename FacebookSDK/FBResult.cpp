#include "pch.h"
#include "FBResult.h"

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;

namespace winsdkfb
{
	FBResult::FBResult(shared_ptr<FBError> error)
		: _error(error)
		, _succeeded(false)
	{
		
	}

	bool FBResult::Succeeded()
	{
		return _succeeded;
	}

	shared_ptr<FBError> FBResult::ErrorInfo()
	{
		return _error;
	}
}
