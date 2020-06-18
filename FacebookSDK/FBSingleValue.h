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
		FBSingleValue(winrt::hstring const& request, std::unordered_map<winrt::hstring, winrt::hstring> parameters, winsdkfb::JsonClassFactory objectFactory);

		concurrency::task<winsdkfb::FBResult> GetAsync();
		concurrency::task<winsdkfb::FBResult> PostAsync();
		concurrency::task<winsdkfb::FBResult> DeleteAsync();

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

		concurrency::task<winsdkfb::FBResult> MakeHttpRequest(
			winsdkfb::HttpMethod httpMethod
		);

		winsdkfb::FBResult _result;
		std::wstring _request;
		std::unordered_map<winrt::hstring, winrt::hstring> _parameters;
		winsdkfb::JsonClassFactory _objectFactory{ nullptr };
	};
}
