#pragma once

#include "FBError.h"

namespace winsdkfb
{
	struct FBResult
	{
	public:
		FBResult() = default;
		FBResult(winsdkfb::FBError error);
		~FBResult() = default;

		bool Succeeded();
		winsdkfb::FBError ErrorInfo();

	protected:
		bool _succeeded = false;
		winsdkfb::FBError _error;
	};
}
