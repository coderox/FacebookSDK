#pragma once

#include "FacebookError.h"
#include "FacebookResult.g.h"

namespace winrt::FacebookSDK::implementation
{
    struct FacebookResult : FacebookResultT<FacebookResult>
    {
	public:
		FacebookResult() { };
        FacebookResult(Windows::Foundation::IInspectable const& Object);

        bool Succeeded();
        Windows::Foundation::IInspectable Object();
        FacebookSDK::FacebookError ErrorInfo();
	private:
		~FacebookResult();

		Windows::Foundation::IInspectable _object{ nullptr };
		FacebookSDK::FacebookError _error{ nullptr };
    };
}

namespace winrt::FacebookSDK::factory_implementation
{
    struct FacebookResult : FacebookResultT<FacebookResult, implementation::FacebookResult>
    {
    };
}
