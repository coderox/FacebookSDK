#pragma once

#include "FacebookError.h"
#include "FacebookResult.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookResult : FacebookResultT<FacebookResult>
	{
	public:
		FacebookResult() = default;
		FacebookResult(Windows::Foundation::IInspectable const& Object);

		bool Succeeded();
		Windows::Foundation::IInspectable Object();
		winsdkfb::FacebookError ErrorInfo();
	private:
		~FacebookResult();

		Windows::Foundation::IInspectable _object{ nullptr };
		winsdkfb::FacebookError _error{ nullptr };
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookResult : FacebookResultT<FacebookResult, implementation::FacebookResult>
	{
	};
}
