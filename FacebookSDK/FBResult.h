#pragma once
#include <any>
#include "FBError.h"

namespace winsdkfb
{
	class FBResult
	{
	public:
		FBResult() = default;
		FBResult(std::any object);
		~FBResult();
		
		template<typename T>
		T Object();
		bool Succeeded();
		winsdkfb::FBError ErrorInfo();
	
	private:
		std::any _object{ nullptr };
		winsdkfb::FBError* _error;
	};
}
