#pragma once

#include "FBError.h"
#include "FBResult.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FBResult : FBResultT<FBResult>
	{
	public:
		FBResult() = default;
		FBResult(Windows::Foundation::IInspectable const& Object);
		~FBResult() = default;

		bool Succeeded();
		Windows::Foundation::IInspectable Object();
		winsdkfb::FBError ErrorInfo();

	private:
		Windows::Foundation::IInspectable _object{ nullptr };
		winsdkfb::FBError _error{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBResult : FBResultT<FBResult, implementation::FBResult>
	{
	};
}
