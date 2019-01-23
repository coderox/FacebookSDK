#include "pch.h"
#include "FacebookDialog.h"

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
	}

	void FacebookDialog::InitDialog()
	{
		throw hresult_not_implemented();
	}

	void FacebookDialog::UninitDialog()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IAsyncOperation<FacebookSDK::FacebookResult> FacebookDialog::ShowLoginDialogAsync(Windows::Foundation::Collections::PropertySet const parameters)
	{
		auto handlerStarting =
			TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(
				this, &FacebookDialog::dialogWebView_LoginNavStarting);
		TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs> handlerCompleted =
			TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(
				this, &FacebookDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this,
			&FacebookDialog::BuildLoginDialogUrl), handlerStarting, handlerCompleted, parameters);
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
		Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs> EventHandlerStarting,
		Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs > EventHandlerCompleted,
		Windows::Foundation::Collections::PropertySet parameters
	) {
		throw hresult_not_implemented();
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
