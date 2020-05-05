#pragma once

#include "FBError.h"

namespace winsdkfb
{
	struct FBResult
	{
		FBResult() = default;
		FBResult(winsdkfb::FBError error);
		~FBResult() = default;

		bool Succeeded();
		winsdkfb::FBError ErrorInfo();

		bool _succeeded = false;
		winsdkfb::FBError _error;
	};

	struct FBVectorOfResults : public FBResult
	{
		FBVectorOfResults(std::vector<FBResult> vectorOfResults) 
			: _vectorOfResults(vectorOfResults)
		{
			_succeeded = true;
		}

		std::vector<FBResult> _vectorOfResults;
	};
}
