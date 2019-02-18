#pragma once

#include "FBDialog.g.h"
#include <pplawait.h>

namespace winrt::winsdkfb::implementation
{
	struct FBDialog : FBDialogT<FBDialog>
	{
		FBDialog();

		void InitDialog();
		void UninitDialog();
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const parameters);

		static hstring GetFBServerUrl();
		static void DeleteCookies();

		void CloseDialogButton_OnClick(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Xaml::RoutedEventArgs const& e
		);

	private:
		Windows::Foundation::IAsyncOperation<winsdkfb::FBResult> ShowDialog(
			DialogUriBuilder const& uriBuilder,
			Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs> EventHandlerStarting,
			Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs > EventHandlerCompleted,
			Windows::Foundation::Collections::PropertySet parameters
		);

		winrt::hstring GetRedirectUriString(
			winrt::hstring DialogName
		);

		static BOOL IsMobilePlatform();

		Windows::Foundation::Uri BuildLoginDialogUrl(
			Windows::Foundation::Collections::PropertySet const& Parameters
		);

		Windows::Foundation::Uri BuildFeedDialogUrl(
			Windows::Foundation::Collections::PropertySet const& Parameters
		);

		Windows::Foundation::Uri BuildRequestsDialogUrl(
			Windows::Foundation::Collections::PropertySet const& Parameters
		);

		Windows::Foundation::Uri BuildSendDialogUrl(
			Windows::Foundation::Collections::PropertySet const& Parameters
		);

		void dialogWebView_LoginNavStarting(
			Windows::UI::Xaml::Controls::WebView const& sender,
			Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_FeedNavStarting(
			Windows::UI::Xaml::Controls::WebView const& sender,
			Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_RequestNavStarting(
			Windows::UI::Xaml::Controls::WebView const sender,
			Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_SendNavStarting(
			Windows::UI::Xaml::Controls::WebView const& sender,
			Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_NavCompleted(
			Windows::UI::Xaml::Controls::WebView const& sender,
			Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
		);

		bool IsLoginSuccessRedirect(
			Windows::Foundation::Uri const&  Response
		);

		bool IsLogoutRedirect(
			Windows::Foundation::Uri const&  Response
		);

		bool IsDialogCloseRedirect(
			Windows::Foundation::Uri const&  Response
		);

		void OnSizeChanged(
			Windows::UI::Core::CoreWindow const& sender,
			Windows::UI::Core::WindowSizeChangedEventArgs const& args
		);

		void SetDialogResponse(winsdkfb::FBResult dialogResponse);

		winrt::event_token navigatingStartingEventHandlerRegistrationToken;
		winrt::event_token navigatingCompletedEventHandlerRegistrationToken;
		winrt::event_token sizeChangedEventRegistrationToken;
		Windows::UI::Xaml::Controls::Grid _grid{ nullptr };
		Windows::UI::Xaml::Controls::Primitives::Popup _popup{ nullptr };
		winsdkfb::FBResult _dialogResponse{ nullptr };
		HANDLE _dialogEventHandle;
	};
}

namespace winrt::winsdkfb::factory_implementation
{
	struct FBDialog : FBDialogT<FBDialog, implementation::FBDialog>
	{
	};
}
