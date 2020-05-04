#pragma once

#include <winrt/base.h>

namespace winsdkfb
{
	struct FBMediaObject : winrt::implements<FBMediaObject, winrt::Windows::Foundation::IActivationFactory, winrt::Windows::Foundation::IInspectable>
	{
		FBMediaObject() = default;

		winrt::hstring ContentType();
		void ContentType(winrt::hstring const& value);
		winrt::hstring FileName();
		void FileName(winrt::hstring const& value);
		void SetValue(std::vector<uint8_t> value);
		std::vector<uint8_t> GetValue();
	
		winrt::hstring GetRuntimeClassName() const
		{
			return L"winsdkfb.FBMediaObject";
		}
		auto ActivateInstance() const
		{
			return winrt::make<FBMediaObject>();
		}

	private:
		std::vector<uint8_t> _value;
		std::wstring _contentType;
		std::wstring _fileName;
	};
}
