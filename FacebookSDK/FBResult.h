#pragma once

#include "FBError.h"

namespace winsdkfb
{
	struct FBResult
	{
	public:
		FBResult() = default;
		FBResult(std::shared_ptr<winsdkfb::FBError> error);
		~FBResult() = default;

		bool Succeeded();
		std::shared_ptr<winsdkfb::FBError> ErrorInfo();

	protected:
		bool _succeeded = false;
		std::shared_ptr<winsdkfb::FBError> _error{ nullptr };
	};
}
