#pragma once

#include <pplawait.h>

#include "HttpMethod.h"
#include "FBResult.h"
#include "Utilities.h"

namespace winsdkfb
{
	struct FBSingleValue : public FBResult
	{
	public:
		FBSingleValue(std::wstring const& request, std::unordered_map<std::wstring, std::wstring> parameters, winsdkfb::JsonClassFactory objectFactory);

		concurrency::task<winsdkfb::FBResult> GetAsync() const;
		concurrency::task<winsdkfb::FBResult> PostAsync() const;
		concurrency::task<winsdkfb::FBResult> DeleteAsync() const;

	private:
		/**
		 * Attempts to use ObjectFactory to create an object from values
		 * contained in JsonText.
		 * @param JsonText Serialized json text that contains values used to
		 * instantiate an object of the type created by ObjectFactory.
		 * @return FBResult that contains a new object created by
		 * ObjectFactory. On failure, the FBResult will instead contain an
		 * FBError object.
		 * @exception InvalidArgumentException if ObjectFactory is unable
		 * to instantiate an object or if the JsonText can't be parsed.
		 */
		winsdkfb::FBResult ConsumeSingleValue(
			std::wstring JsonText
		) const;

		concurrency::task<winsdkfb::FBResult> MakeHttpRequest(
			winsdkfb::HttpMethod httpMethod
		) const;

		winsdkfb::FBResult _result;
		std::wstring _request;
		std::unordered_map<std::wstring, std::wstring> _parameters;
		winsdkfb::JsonClassFactory _objectFactory{ nullptr };
	};
}
