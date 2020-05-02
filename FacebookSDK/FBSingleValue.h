#pragma once

#include "HttpMethod.h"
#include "FBResult.h"
#include "Utilities.h"

namespace winsdkfb
{
	struct FBSingleValue
	{
	public:
		FBSingleValue(winrt::hstring const& request, winrt::Windows::Foundation::Collections::PropertySet const& parameters, winsdkfb::JsonClassFactory const& objectFactory);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> GetAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> PostAsync();
		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> DeleteAsync();

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
		winsdkfb::FBResult ConsumeSingleValue(
			winrt::hstring JsonText
		);

		winrt::Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> MakeHttpRequest(
			::winsdkfb::HttpMethod httpMethod
		);

		winsdkfb::FBResult _result{ nullptr };
		std::wstring _request;
		winrt::Windows::Foundation::Collections::PropertySet _parameters;
		winsdkfb::JsonClassFactory _objectFactory;
	};
}
