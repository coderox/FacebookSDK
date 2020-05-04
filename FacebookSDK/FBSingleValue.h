#pragma once

#include <pplawait.h>

#include "HttpMethod.h"
#include "FBResult.h"
#include "Utilities.h"

namespace winsdkfb
{
	//struct JsonClassFactory
	//{
	//	//winrt::Windows::Foundation::IInspectable operator()(winrt::hstring const& d);
	//};

	struct FBSingleValue : public FBResult
	{
	public:
		FBSingleValue(winrt::hstring const& request, winrt::Windows::Foundation::Collections::PropertySet const& parameters, winsdkfb::JsonClassFactory objectFactory);

		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> GetAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> PostAsync();
		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> DeleteAsync();

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
		std::shared_ptr<winsdkfb::FBResult> ConsumeSingleValue(
			winrt::hstring JsonText
		);

		concurrency::task<std::shared_ptr<winsdkfb::FBResult>> MakeHttpRequest(
			winsdkfb::HttpMethod httpMethod
		);

		std::shared_ptr<winsdkfb::FBResult> _result{ nullptr };
		std::wstring _request;
		winrt::Windows::Foundation::Collections::PropertySet _parameters;
		winsdkfb::JsonClassFactory _objectFactory{ nullptr };
	};
}
