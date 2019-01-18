#pragma once

#include "FacebookDialog.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookDialog : FacebookDialogT<FacebookDialog>
	{
		FacebookDialog() = default;

		void InitDialog();
		void UninitDialog();
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);

		static hstring GetFBServerUrl();
		static void DeleteCookies();
	
		void CloseDialogButton_OnClick(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Xaml::RoutedEventArgs const& e
		) {
			throw hresult_not_implemented();
		}
	private:

	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookDialog : FacebookDialogT<FacebookDialog, implementation::FacebookDialog>
	{
	};
}
