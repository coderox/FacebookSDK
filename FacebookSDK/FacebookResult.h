#pragma once

#include "FacebookError.h"
#include "FacebookResult.g.h"

namespace winrt::FacebookSDK::implementation
{
    struct FacebookResult : FacebookResultT<FacebookResult>
    {
	public:
        FacebookResult() = delete;
        FacebookResult(Windows::Foundation::IInspectable const& Object);

        bool Succeeded();
        Windows::Foundation::IInspectable Object();
        FacebookSDK::FacebookError ErrorInfo();
	private:
		~FacebookResult();

		Windows::Foundation::IInspectable _object;
		FacebookSDK::FacebookError _error;
    };
}

namespace winrt::FacebookSDK::factory_implementation
{
    struct FacebookResult : FacebookResultT<FacebookResult, implementation::FacebookResult>
    {
    };
}
