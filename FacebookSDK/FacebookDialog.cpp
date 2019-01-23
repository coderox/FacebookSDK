#include "pch.h"
#include "FacebookDialog.h"
#include "FacebookSession.h"
#include "FacebookFeedRequest.h"
#include "FacebookAppRequest.h"
#include "HttpManager.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#define FACEBOOK_LOGOUT_PATH  L"/logout.php"
#define FACEBOOK_DIALOG_CLOSE_PATH   L"/dialog/close"

const wchar_t* ErrorObjectJson = L"{\"error\": {\"message\": " \
L"\"Operation Canceled\", \"type\": " \
L"\"OAuthException\", \"code\": 4201, " \
L"\"error_user_msg\": \"User canceled the Dialog flow\"" \
L"}}";

const wchar_t* ErrorObjectJsonLogout = L"{\"error\": {\"message\": " \
L"\"Operation Canceled\", \"type\": " \
L"\"OAuthException\", \"code\": 4202, " \
L"\"error_user_msg\": \"User logged out\"" \
L"}}";

const wchar_t* ErrorObjectJsonNoInternet = L"{\"error\": {\"message\": " \
L"\"Operation Canceled\", \"type\": " \
L"\"OAuthException\", \"code\": 4203, " \
L"\"error_user_msg\": \"No Internet\"" \
L"}}";

namespace winrt::FacebookSDK::implementation
{
	FacebookDialog::FacebookDialog()
	{
		InitializeComponent();
		InitDialog();
	}

	void FacebookDialog::InitDialog()
	{
		_popup = nullptr;

		CoreWindow coreWindow = CoreApplication::MainView().CoreWindow();

		_popup = Popup();

		_popup.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Stretch);
		_popup.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Stretch);

		Margin(Windows::UI::Xaml::Thickness({ 0, 0, 0, 0 }));
		Height(coreWindow.Bounds().Height);
		Width(coreWindow.Bounds().Width);

		sizeChangedEventRegistrationToken = coreWindow.SizeChanged(
			TypedEventHandler<CoreWindow, WindowSizeChangedEventArgs>
			(this, &FacebookDialog::OnSizeChanged));

		_popup.Child(*this);
	}

	void FacebookDialog::UninitDialog()
	{
		dialogWebBrowser().Stop();
		dialogWebBrowser().NavigationStarting(navigatingStartingEventHandlerRegistrationToken);
		dialogWebBrowser().NavigationCompleted(navigatingCompletedEventHandlerRegistrationToken);
		CoreApplication::MainView().CoreWindow().SizeChanged(sizeChangedEventRegistrationToken);

		_popup.IsOpen(false);

		//
		// This breaks the circular dependency between the popup and dialog
		// class, and is essential in order for the dialog to be disposed of
		// properly.
		//
		_popup.Child(nullptr);
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowLoginDialogAsync(PropertySet const parameters)
	{
		auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FacebookDialog::dialogWebView_LoginNavStarting);
		auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FacebookDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this, &FacebookDialog::BuildLoginDialogUrl), handlerStarting, handlerCompleted, parameters);
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowFeedDialogAsync(Windows::Foundation::Collections::PropertySet const parameters)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowRequestsDialogAsync(Windows::Foundation::Collections::PropertySet const parameters)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowSendDialogAsync(Windows::Foundation::Collections::PropertySet const parameters)
	{
		throw hresult_not_implemented();
	}

	hstring FacebookDialog::GetFBServerUrl()
	{
		throw hresult_not_implemented();
	}

	void FacebookDialog::DeleteCookies()
	{
		throw hresult_not_implemented();
	}

	IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowDialog(
		DialogUriBuilder const& uriBuilder,
		TypedEventHandler<WebView, WebViewNavigationStartingEventArgs> eventHandlerStarting,
		TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs > eventHandlerCompleted,
		PropertySet parameters
	) {
		Uri dialogUrl = uriBuilder(parameters);

		navigatingStartingEventHandlerRegistrationToken = dialogWebBrowser().NavigationStarting(eventHandlerStarting);
		navigatingCompletedEventHandlerRegistrationToken = dialogWebBrowser().NavigationCompleted(eventHandlerCompleted);
		_popup.IsOpen(true);

		dialogWebBrowser().Navigate(dialogUrl);
		dialogWebBrowser().Focus(Windows::UI::Xaml::FocusState::Programmatic);

		co_return _dialogResponse;
		//return create_async([=]()
		//{
		//	return create_task(_dialogResponse);
		//});
	}

	winrt::hstring FacebookDialog::GetRedirectUriString(
		winrt::hstring DialogName
	) {
		throw hresult_not_implemented();
	}

	BOOL FacebookDialog::IsMobilePlatform() {
		throw hresult_not_implemented();
	}

	Windows::Foundation::Uri FacebookDialog::BuildLoginDialogUrl(
		Windows::Foundation::Collections::PropertySet const& Parameters
	) {
		throw hresult_not_implemented();
	}

	Windows::Foundation::Uri FacebookDialog::BuildFeedDialogUrl(
		Windows::Foundation::Collections::PropertySet const& Parameters
	) {
		throw hresult_not_implemented();
	}

	Windows::Foundation::Uri FacebookDialog::BuildRequestsDialogUrl(
		Windows::Foundation::Collections::PropertySet const& Parameters
	) {
		throw hresult_not_implemented();
	}

	Windows::Foundation::Uri FacebookDialog::BuildSendDialogUrl(
		Windows::Foundation::Collections::PropertySet const& Parameters
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::dialogWebView_LoginNavStarting(
		Windows::UI::Xaml::Controls::WebView const& sender,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::dialogWebView_FeedNavStarting(
		Windows::UI::Xaml::Controls::WebView const& sender,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::dialogWebView_RequestNavStarting(
		Windows::UI::Xaml::Controls::WebView const sender,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::dialogWebView_SendNavStarting(
		Windows::UI::Xaml::Controls::WebView const& sender,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::dialogWebView_NavCompleted(
		Windows::UI::Xaml::Controls::WebView const& sender,
		Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::CloseDialogButton_OnClick(
		Windows::Foundation::IInspectable const& sender,
		Windows::UI::Xaml::RoutedEventArgs const& e
	) {
		throw hresult_not_implemented();
	}

	bool FacebookDialog::IsLoginSuccessRedirect(
		Windows::Foundation::Uri const&  Response
	) {
		throw hresult_not_implemented();
	}

	bool FacebookDialog::IsLogoutRedirect(
		Windows::Foundation::Uri const&  Response
	) {
		throw hresult_not_implemented();
	}

	bool FacebookDialog::IsDialogCloseRedirect(
		Windows::Foundation::Uri const&  Response
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::OnSizeChanged(
		Windows::UI::Core::CoreWindow const& sender,
		Windows::UI::Core::WindowSizeChangedEventArgs const& args
	) {
		throw hresult_not_implemented();
	}

	void FacebookDialog::SetDialogResponse(FacebookSDK::FacebookResult dialogResponse) {
		throw hresult_not_implemented();
	}
}
