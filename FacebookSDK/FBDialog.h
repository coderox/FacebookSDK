#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <Windows.h>
#include "Utilities.h"
#include <pplawait.h>

namespace winsdkfb
{
	struct FBDialog 
		: winrt::implements<
			FBDialog,
			winrt::composing,
			winrt::Windows::UI::Xaml::Controls::IControlOverrides,
			winrt::Windows::UI::Xaml::Controls::IControlOverrides6,
			winrt::Windows::UI::Xaml::IFrameworkElementOverrides,
			winrt::Windows::UI::Xaml::IFrameworkElementOverrides2,
			winrt::Windows::UI::Xaml::IUIElementOverrides,
			winrt::Windows::UI::Xaml::IUIElementOverrides7,
			winrt::Windows::UI::Xaml::IUIElementOverrides8,
			winrt::Windows::UI::Xaml::IUIElementOverrides9>,
		winrt::impl::require <
			FBDialog,
			winrt::Windows::UI::Xaml::Controls::IUserControl,
			winrt::Windows::UI::Xaml::Controls::IControl,
			winrt::Windows::UI::Xaml::Controls::IControl2,
			winrt::Windows::UI::Xaml::Controls::IControl3,
			winrt::Windows::UI::Xaml::Controls::IControl4,
			winrt::Windows::UI::Xaml::Controls::IControl5,
			winrt::Windows::UI::Xaml::Controls::IControl7,
			winrt::Windows::UI::Xaml::Controls::IControlProtected,
			winrt::Windows::UI::Xaml::IFrameworkElement,
			winrt::Windows::UI::Xaml::IFrameworkElement2,
			winrt::Windows::UI::Xaml::IFrameworkElement3,
			winrt::Windows::UI::Xaml::IFrameworkElement4,
			winrt::Windows::UI::Xaml::IFrameworkElement6,
			winrt::Windows::UI::Xaml::IFrameworkElement7,
			winrt::Windows::UI::Xaml::IFrameworkElementProtected7,
			winrt::Windows::UI::Xaml::IUIElement,
			winrt::Windows::UI::Xaml::IUIElement2,
			winrt::Windows::UI::Xaml::IUIElement3,
			winrt::Windows::UI::Xaml::IUIElement4,
			winrt::Windows::UI::Xaml::IUIElement5,
			winrt::Windows::UI::Xaml::IUIElement7,
			winrt::Windows::UI::Xaml::IUIElement8,
			winrt::Windows::UI::Xaml::IUIElement9,
			winrt::Windows::UI::Xaml::IUIElement10,
			winrt::Windows::UI::Composition::IAnimationObject,
			winrt::Windows::UI::Composition::IVisualElement,
			winrt::Windows::UI::Xaml::IDependencyObject,
			winrt::Windows::UI::Xaml::IDependencyObject2 > ,
		winrt::impl::base<
			FBDialog,
			winrt::Windows::UI::Xaml::Controls::UserControl,
			winrt::Windows::UI::Xaml::Controls::Control,
			winrt::Windows::UI::Xaml::FrameworkElement,
			winrt::Windows::UI::Xaml::UIElement,
			winrt::Windows::UI::Xaml::DependencyObject>,
			winrt::Windows::UI::Xaml::Controls::IControlOverridesT<FBDialog>,
			winrt::Windows::UI::Xaml::Controls::IControlOverrides6T<FBDialog>,
			winrt::Windows::UI::Xaml::IFrameworkElementOverridesT<FBDialog>,
			winrt::Windows::UI::Xaml::IFrameworkElementOverrides2T<FBDialog>,
			winrt::Windows::UI::Xaml::IUIElementOverridesT<FBDialog>,
			winrt::Windows::UI::Xaml::IUIElementOverrides7T<FBDialog>,
			winrt::Windows::UI::Xaml::IUIElementOverrides8T<FBDialog>,
			winrt::Windows::UI::Xaml::IUIElementOverrides9T<FBDialog>
	{
		winrt::hstring GetRuntimeClassName() const
		{
			return L"BlankApp1.BlankUserControl";
		}

		//void InitializeComponent();

		//void Connect(
		//	int connectionId, winrt::Windows::Foundation::IInspectable target
		//);

		FBDialog();
		~FBDialog() { }

	//	void InitDialog();
	//	void UninitDialog();
	//	concurrency::task<winsdkfb::FBResult> ShowLoginDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
	//	concurrency::task<winsdkfb::FBResult> ShowFeedDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
	//	concurrency::task<winsdkfb::FBResult> ShowRequestsDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);
	//	concurrency::task<winsdkfb::FBResult> ShowSendDialogAsync(winrt::Windows::Foundation::Collections::PropertySet const parameters);

	//	static winrt::hstring GetFBServerUrl();
	//	static void DeleteCookies();

	//	void CloseDialogButton_OnClick(
	//		winrt::Windows::Foundation::IInspectable const& sender,
	//		winrt::Windows::UI::Xaml::RoutedEventArgs const& e
	//	);

	//private:
	//	concurrency::task<winsdkfb::FBResult> ShowDialog(
	//		winsdkfb::DialogUriBuilder const& uriBuilder,
	//		//winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs> EventHandlerStarting,
	//		//winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, winrt::Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs > EventHandlerCompleted,
	//		winrt::Windows::Foundation::Collections::PropertySet parameters
	//	);

	//	winrt::hstring GetRedirectUriString(
	//		winrt::hstring DialogName
	//	);

	//	static bool IsMobilePlatform();

	//	winrt::Windows::Foundation::Uri BuildLoginDialogUrl(
	//		winrt::Windows::Foundation::Collections::PropertySet const& Parameters
	//	);

	//	winrt::Windows::Foundation::Uri BuildFeedDialogUrl(
	//		winrt::Windows::Foundation::Collections::PropertySet const& Parameters
	//	);

	//	winrt::Windows::Foundation::Uri BuildRequestsDialogUrl(
	//		winrt::Windows::Foundation::Collections::PropertySet const& Parameters
	//	);

	//	winrt::Windows::Foundation::Uri BuildSendDialogUrl(
	//		winrt::Windows::Foundation::Collections::PropertySet const& Parameters
	//	);

	//	void dialogWebView_LoginNavStarting(
	//		winrt::Windows::UI::Xaml::Controls::WebView const& sender,
	//		winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	//	);

	//	void dialogWebView_FeedNavStarting(
	//		winrt::Windows::UI::Xaml::Controls::WebView const& sender,
	//		winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	//	);

	//	void dialogWebView_RequestNavStarting(
	//		winrt::Windows::UI::Xaml::Controls::WebView const sender,
	//		winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	//	);

	//	void dialogWebView_SendNavStarting(
	//		winrt::Windows::UI::Xaml::Controls::WebView const& sender,
	//		winrt::Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	//	);

	//	void dialogWebView_NavCompleted(
	//		winrt::Windows::UI::Xaml::Controls::WebView const& sender,
	//		winrt::Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
	//	);

	//	bool IsLoginSuccessRedirect(
	//		winrt::Windows::Foundation::Uri const&  Response
	//	);

	//	bool IsLogoutRedirect(
	//		winrt::Windows::Foundation::Uri const&  Response
	//	);

	//	bool IsDialogCloseRedirect(
	//		winrt::Windows::Foundation::Uri const&  Response
	//	);

	//	void OnSizeChanged(
	//		winrt::Windows::UI::Core::CoreWindow const& sender,
	//		winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args
	//	);

	//	void SetDialogResponse(winsdkfb::FBResult dialogResponse);

	//	winrt::event_token navigatingStartingEventHandlerRegistrationToken;
	//	winrt::event_token navigatingCompletedEventHandlerRegistrationToken;
	//	winrt::event_token sizeChangedEventRegistrationToken;
	//	winrt::Windows::UI::Xaml::Controls::Grid _grid{ nullptr };
	//	winrt::Windows::UI::Xaml::Controls::Primitives::Popup _popup{ nullptr };
	//	winsdkfb::FBResult _dialogResponse;
	//	HANDLE _dialogEventHandle;
	//	bool _contentLoaded = false;
	};
}
