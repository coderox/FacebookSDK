#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
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
		void UnInitDialog() const;

		concurrency::task<winsdkfb::FBResult> ShowLoginDialogAsync(std::unordered_map<std::wstring, std::wstring> const parameters);
		concurrency::task<winsdkfb::FBResult> ShowFeedDialogAsync(std::unordered_map<std::wstring, std::wstring> const parameters);
		concurrency::task<winsdkfb::FBResult> ShowRequestsDialogAsync(std::unordered_map<std::wstring, std::wstring> const parameters);
		concurrency::task<winsdkfb::FBResult> ShowSendDialogAsync(std::unordered_map<std::wstring, std::wstring> const parameters);

		static std::wstring GetFBServerUrl();
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
			std::unordered_map<std::wstring, std::wstring> parameters
		);

		static std::wstring GetRedirectUriString(
			std::wstring DialogName
		);

		static bool IsMobilePlatform();

		static winrt::Windows::Foundation::Uri BuildLoginDialogUrl(
			std::unordered_map<std::wstring, std::wstring> const Parameters
		);

		static winrt::Windows::Foundation::Uri BuildFeedDialogUrl(
			std::unordered_map<std::wstring, std::wstring> const Parameters
		);

		static winrt::Windows::Foundation::Uri BuildRequestsDialogUrl(
			std::unordered_map<std::wstring, std::wstring> const Parameters
		);

		static winrt::Windows::Foundation::Uri BuildSendDialogUrl(
			std::unordered_map<std::wstring, std::wstring> const Parameters
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

		static bool IsLoginSuccessRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		static bool IsLogoutRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		static bool IsDialogCloseRedirect(
			winrt::Windows::Foundation::Uri const&  Response
		);

		void OnSizeChanged(
			winrt::Windows::UI::Core::CoreWindow const& sender,
			winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args
		) const;

		void SetDialogResponse(winsdkfb::FBResult dialogResponse);

		winrt::event_token navigatingStartingEventHandlerRegistrationToken;
		winrt::event_token navigatingCompletedEventHandlerRegistrationToken;
		winrt::event_token sizeChangedEventRegistrationToken;
		winrt::Windows::UI::Xaml::Controls::WebView dialogWebBrowser;
		winrt::Windows::UI::Xaml::Controls::Button closeDialogButton;
		winrt::Windows::UI::Xaml::Controls::Primitives::Popup popup{ nullptr };
		winsdkfb::FBResult _dialogResponse;
		HANDLE dialogEventHandle;
		bool contentLoaded = false;
	};
}
