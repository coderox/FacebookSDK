#pragma once
#include <any>
#include "FBError.h"

namespace winsdkfb
{
	class FBResult
	{
	public:
		FBResult();
		FBResult(std::any&& object);
		~FBResult();
		
		template<typename To> std::optional<To> Object() 
		{
			try
			{
				return std::any_cast<To>(_object);
			}
			catch (...)
			{
				return {};
			}

		}

		bool Succeeded();
		winsdkfb::FBError ErrorInfo();
	
	private:
		std::any _object{ nullptr };
		std::any _error{ nullptr };
	};
}
