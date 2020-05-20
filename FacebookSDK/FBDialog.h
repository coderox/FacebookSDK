#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <Windows.h>
#include "Utilities.h"
#include <pplawait.h>

namespace winsdkfb
{
	struct FBDialog : winrt::Windows::UI::Xaml::Controls::UserControlT<FBDialog>
	{
		void InitializeComponent();

		FBDialog();
		~FBDialog() { }

		void InitDialog();
		void UninitDialog();

		concurrency::task<winsdkfb::FBResult> ShowLoginDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
		concurrency::task<winsdkfb::FBResult> ShowFeedDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
		concurrency::task<winsdkfb::FBResult> ShowRequestsDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
		concurrency::task<winsdkfb::FBResult> ShowSendDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);

		static winrt::hstring GetFBServerUrl();
		static void DeleteCookies();

		void CloseDialogButton_OnClick(
			winrt::Windows::Foundation::IInspectable const& sender,
			winrt::Windows::UI::Xaml::RoutedEventArgs const& e
		);

	private:

		concurrency::task<winsdkfb::FBResult> ShowDialog(
			winsdkfb::DialogUriBuilder const& uriBuilder,
			winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs> const& EventHandlerStarting,
			winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, winrt::Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs > const& EventHandlerCompleted,
			winrt::Windows::Foundation::Collections::PropertySet const& parameters
		);

		winrt::hstring GetRedirectUriString(
			winrt::hstring DialogName
		);

		static bool IsMobilePlatform();

		winrt::Windows::Foundation::Uri BuildLoginDialogUrl(
			winrt::Windows::Foundation::Collections::PropertySet const& Parameters
		);

		winrt::Windows::Foundation::Uri BuildFeedDialogUrl(
			winrt::Windows::Foundation::Collections::PropertySet const& Parameters
		);

		winrt::Windows::Foundation::Uri BuildRequestsDialogUrl(
			winrt::Windows::Foundation::Collections::PropertySet const& Parameters
		);

		winrt::Windows::Foundation::Uri BuildSendDialogUrl(
			winrt::Windows::Foundation::Collections::PropertySet const& Parameters
		);

		void dialogWebView_LoginNavStarting(
			winrt::Windows::UI::Xaml::Controls::WebView const& sender,
			winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_FeedNavStarting(
			winrt::Windows::UI::Xaml::Controls::WebView const& sender,
			winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_RequestNavStarting(
			winrt::Windows::UI::Xaml::Controls::WebView const sender,
			winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_SendNavStarting(
			winrt::Windows::UI::Xaml::Controls::WebView const& sender,
			winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
		);

		void dialogWebView_NavCompleted(
			winrt::Windows::UI::Xaml::Controls::WebView const& sender,
			winrt::Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
		);

		bool IsLoginSuccessRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		bool IsLogoutRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		bool IsDialogCloseRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		void OnSizeChanged(
			winrt::Windows::UI::Core::CoreWindow const& sender,
			winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args
		);

		void SetDialogResponse(winsdkfb::FBResult dialogResponse);

		winrt::event_token navigatingStartingEventHandlerRegistrationToken;
		winrt::event_token navigatingCompletedEventHandlerRegistrationToken;
		winrt::event_token sizeChangedEventRegistrationToken;
		winrt::Windows::UI::Xaml::Controls::WebView dialogWebBrowser;
		winrt::Windows::UI::Xaml::Controls::Button closeDialogButton;
		winrt::Windows::UI::Xaml::Controls::Primitives::Popup popup{ nullptr };
		winsdkfb::FBResult dialogResponse;
		HANDLE dialogEventHandle;
		bool contentLoaded = false;
	};
}
