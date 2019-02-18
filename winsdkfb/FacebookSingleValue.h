#pragma once

#include "HttpMethod.h"
#include "FacebookResult.h"
#include "FacebookSingleValue.g.h"

namespace winrt::winsdkfb::implementation
{
	struct FacebookSingleValue : FacebookSingleValueT<FacebookSingleValue>
	{
	public:
		FacebookSingleValue(hstring const& request, Windows::Foundation::Collections::PropertySet const& parameters, winsdkfb::JsonClassFactory const& objectFactory);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> GetAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> PostAsync();
		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> DeleteAsync();

	private:
		/**
		 * Attempts to use ObjectFactory to create an object from values
		 * contained in JsonText.
		 * @param JsonText Serialized json text that contains values used to
		 * instantiate an object of the type created by ObjectFactory.
		 * @return FBResult that contains a new object created by
		 * ObjectFactory. On failure, the FBResult will instead contain an
		 * FBError object.
		 * @exception InvalidArgumentException if ObjectyFactory is unable
		 * to instantiate an object or if the JsonText is unparsable.
		 */
		winsdkfb::FacebookResult ConsumeSingleValue(
			winrt::hstring JsonText
		);

		Windows::Foundation::IAsyncOperation<winsdkfb::FacebookResult> MakeHttpRequest(
			::winsdkfb::HttpMethod httpMethod
		);

		winsdkfb::FacebookResult _result{ nullptr };
		std::wstring _request;
		winrt::Windows::Foundation::Collections::PropertySet _parameters;
		winsdkfb::JsonClassFactory _objectFactory;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FacebookSingleValue : FacebookSingleValueT<FacebookSingleValue, implementation::FacebookSingleValue>
	{
	};
}
