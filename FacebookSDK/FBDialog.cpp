#include "FBDialog.h"
#include "FBSession.h"
#include "FBResult.h"
#include "FBFeedRequest.h"
#include "FBAppRequest.h"
#include "HttpManager.h"
#include "Utilities.h"
#include "FBConstants.h"

#include <sstream>
#include <string>

#include "winrt/Windows.Web.Http.Filters.h"
#include "winrt/Windows.ApplicationModel.Core.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Windows.UI.Core.h"

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

#define ScopeKey        L"scope"
#define DisplayKey      L"display"
#define ResponseTypeKey L"response_type"
#define DefaultScope    L"public_profile,email,user_friends"
#define DefaultDisplay  L"popup"
#define DefaultResponse L"token"

#ifdef _DEBUG
void DebugPrintLine(
	hstring const& msg
)
{
	hstring output(msg + L"\n");
	OutputDebugString(output.c_str());
}
#else
#define DebugPrintLine(msg) ((void) 0)
#endif

namespace winsdkfb
{
	FBDialog::FBDialog()
	{
		//InitializeComponent();
		//InitDialog();
	}

//	void FBDialog::InitializeComponent() {
//		if (_contentLoaded)
//			return;
//
//		_contentLoaded = true;
//
//		//LayoutRoot = ref new Windows::UI::Xaml::Controls::Grid();
//		//Windows::UI::Xaml::Media::SolidColorBrush^ backgroundBrush = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Black);
//		//backgroundBrush->Opacity = 0.6;
//		//LayoutRoot->Background = backgroundBrush;
//
//		//_browserGrid = ref new Windows::UI::Xaml::Controls::Grid();
//
//		//dialogWebBrowser = ref new Windows::UI::Xaml::Controls::WebView();
//		//dialogWebBrowser->Margin = Thickness(30);
//		//dialogWebBrowser->NavigationStarting += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView^, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^>(this, &winsdkfb::FacebookDialog::OnNavigationStarting);
//
//		//closeDialogButton = ref new Windows::UI::Xaml::Controls::Button();
//		//closeDialogButton->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Right;
//		//closeDialogButton->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
//		//closeDialogButton->Background = ref new SolidColorBrush(Windows::UI::Colors::Transparent);
//		//closeDialogButton->Width = 30;
//		//closeDialogButton->Height = 30;
//		//closeDialogButton->Margin = Thickness(15);
//
//		//closeDialogButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
//		//closeDialogButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &FacebookDialog::CloseDialogButton_OnClick);
//		//dialogWebBrowser->NavigationCompleted += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView^, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^>(([=](WebView^, WebViewNavigationCompletedEventArgs^) {
//		//	closeDialogButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
//		//	}));
//
//		//auto style = (Windows::UI::Xaml::Style^)Windows::UI::Xaml::Markup::XamlReader::Load(L"<Style xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation' TargetType='Button'>" +
//		//	"<Setter Property='Background' Value='Transparent'/>" +
//		//	"<Setter Property='Template'>" +
//		//	"<Setter.Value>" +
//		//	"<ControlTemplate TargetType='Button'>" +
//		//	"<Viewbox Width='30' Height='30' Margin='0' HorizontalAlignment='Right' VerticalAlignment='Top'>" +
//		//	"<Canvas Width='148.981' Height='148.981'>" +
//		//	"<Ellipse Width='110.986' Height='110.986' Canvas.Left='18.9976' Canvas.Top='18.9976' Stretch='Fill' StrokeThickness='0.999875' StrokeLineJoin='Round' Stroke='#FF000000' Fill='#FF000000' />" +
//		//	"<Ellipse Width='148.981' Height='148.981' Canvas.Left='-5.95748e-005' Canvas.Top='-2.90573e-005' Stretch='Fill' StrokeThickness='0.999875' StrokeLineJoin='Round' Stroke='#FFFFFFFF' Fill='#FF000000' />" +
//		//	"<Path Width='84.7983' Height='84.7983' Canvas.Left='31.8416' Canvas.Top='32.3415' Stretch='Fill' StrokeThickness='0.999875' StrokeLineJoin='Round' Stroke='#FFFFFFFF' Fill='#FFFFFFFF' Data='F1 M 32.3416,102.499L 102,32.8414L 116.14,46.9818L 46.4819,116.64L 32.3416,102.499 Z ' />" +
//		//	"<Path Width='84.1315' Height='84.1316' Canvas.Left='32.3415' Canvas.Top='32.3416' Stretch='Fill' StrokeThickness='0.999875' StrokeLineJoin='Round' Stroke='#FFFFFFFF' Fill='#FFFFFFFF' Data='F1 M 46.9818,32.8416L 115.973,101.833L 101.833,115.973L 32.8414,46.9819L 46.9818,32.8416 Z ' />" +
//		//	"<Path Width='140.982' Height='140.982' Canvas.Left='3.99957' Canvas.Top='3.99945' Stretch='Fill' StrokeThickness='0.999875' StrokeLineJoin='Round' Stroke='#FFFFFFFF' Fill='#FFFFFFFF' Data='M 74.4907,4.49939C 113.146,4.49939 144.482,35.8356 144.482,74.4906C 144.482,113.146 113.146,144.482 74.4907,144.482C 35.8356,144.482 4.49951,113.146 4.49951,74.4906C 4.49951,35.8356 35.8357,4.49939 74.4907,4.49939 Z M 74.4907,19.4976C 104.863,19.4976 129.484,44.1188 129.484,74.4906C 129.484,104.863 104.863,129.484 74.4907,129.484C 44.1188,129.484 19.4975,104.863 19.4975,74.4906C 19.4975,44.1188 44.1188,19.4976 74.4907,19.4976 Z ' />" +
//		//	"</Canvas>" +
//		//	"</Viewbox>" +
//		//	"</ControlTemplate>" +
//		//	"</Setter.Value>" +
//		//	"</Setter>" +
//		//	"</Style>");
//		//closeDialogButton->Style = style;
//
//		//_browserGrid->Children->Append(dialogWebBrowser);
//		//_browserGrid->Children->Append(closeDialogButton);
//		//LayoutRoot->Children->Append(_browserGrid);
//
//		//this->Content = LayoutRoot;
//	}
//
//	void FBDialog::Connect(int /*connectionId*/, IInspectable /*target*/) {
//		//_contentLoaded = true;
//	}
//
//	void FBDialog::InitDialog()
//	{
//		_popup = nullptr;
//
//		auto coreWindow = CoreApplication::MainView().CoreWindow();
//
//		_popup = Popup();
//
//		_popup.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Stretch);
//		_popup.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Stretch);
//
//		Margin(Thickness({ 0, 0, 0, 0 }));
//		Height(coreWindow.Bounds().Height);
//		Width(coreWindow.Bounds().Width);
//
//		//sizeChangedEventRegistrationToken = coreWindow.SizeChanged(
//		//	TypedEventHandler<CoreWindow, WindowSizeChangedEventArgs>
//		//	(this, &FBDialog::OnSizeChanged));
//
//		//_dialogEventHandle = CreateEvent(nullptr, true, false, nullptr);
//
//		_popup.Child(*this);
//	}
//
//	void FBDialog::UninitDialog()
//	{
//		//dialogWebBrowser().Stop();
//		//dialogWebBrowser().NavigationStarting(navigatingStartingEventHandlerRegistrationToken);
//		//dialogWebBrowser().NavigationCompleted(navigatingCompletedEventHandlerRegistrationToken);
//		//CoreApplication::MainView().CoreWindow().SizeChanged(sizeChangedEventRegistrationToken);
//
//		//_popup.IsOpen(false);
//
//		////
//		//// This breaks the circular dependency between the popup and dialog
//		//// class, and is essential in order for the dialog to be disposed of
//		//// properly.
//		////
//		//_popup.Child(nullptr);
//	}
//
//	concurrency::task<winsdkfb::FBResult> FBDialog::ShowLoginDialogAsync(PropertySet const parameters)
//	{
//		//auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_LoginNavStarting);
//		//auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
//		
//		winsdkfb::DialogUriBuilder uriBuilder = [this](Windows::Foundation::Collections::PropertySet const& parameters) -> Windows::Foundation::Uri {
//			return this->BuildLoginDialogUrl(parameters);
//		};
//
//		return ShowDialog(uriBuilder, parameters);
//	}
//
//	concurrency::task<winsdkfb::FBResult> FBDialog::ShowFeedDialogAsync(PropertySet const parameters)
//	{
//		//auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_FeedNavStarting);
//		//auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
//		//return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildFeedDialogUrl), handlerStarting, handlerCompleted, parameters);
//		winsdkfb::DialogUriBuilder uriBuilder = [this](Windows::Foundation::Collections::PropertySet const& parameters) -> Windows::Foundation::Uri {
//			return this->BuildFeedDialogUrl(parameters);
//		};
//
//		return ShowDialog(uriBuilder, parameters);
//	}
//
//	concurrency::task<winsdkfb::FBResult> FBDialog::ShowRequestsDialogAsync(PropertySet const parameters)
//	{
//		//auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_RequestNavStarting);
//		//auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
//		//return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildRequestsDialogUrl), handlerStarting, handlerCompleted, parameters);
//		winsdkfb::DialogUriBuilder uriBuilder = [this](Windows::Foundation::Collections::PropertySet const& parameters) -> Windows::Foundation::Uri {
//			return this->BuildRequestsDialogUrl(parameters);
//		};
//
//		return ShowDialog(uriBuilder, parameters);
//	}
//
//	concurrency::task<winsdkfb::FBResult> FBDialog::ShowSendDialogAsync(PropertySet const parameters)
//	{
//		//auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_SendNavStarting);
//		//auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
//		//return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildSendDialogUrl), handlerStarting, handlerCompleted, parameters);
//		winsdkfb::DialogUriBuilder uriBuilder = [this](Windows::Foundation::Collections::PropertySet const& parameters) -> Windows::Foundation::Uri {
//			return this->BuildSendDialogUrl(parameters);
//		};
//
//		return ShowDialog(uriBuilder, parameters);
//	}
//
//	hstring FBDialog::GetFBServerUrl()
//	{
//		return FBDialog::IsMobilePlatform() ? FACEBOOK_MOBILE_SERVER_NAME : FACEBOOK_DESKTOP_SERVER_NAME;
//	}
//
//	void FBDialog::DeleteCookies()
//	{
//		HttpBaseProtocolFilter filter;
//		HttpCookieManager cookieManager(filter.CookieManager());
//		HttpCookieCollection cookiesJar(cookieManager.GetCookies(Uri(FBDialog::GetFBServerUrl())));
//		for (auto const& cookie : cookiesJar)
//		{
//			cookieManager.DeleteCookie(cookie);
//		}
//	}
//
//	concurrency::task<winsdkfb::FBResult> FBDialog::ShowDialog(
//		DialogUriBuilder const& uriBuilder,
//		//TypedEventHandler<WebView, WebViewNavigationStartingEventArgs> eventHandlerStarting,
//		//TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs > eventHandlerCompleted,
//		PropertySet parameters
//	) {
//		Uri dialogUrl = uriBuilder(parameters);
//
//		//navigatingStartingEventHandlerRegistrationToken = dialogWebBrowser().NavigationStarting(eventHandlerStarting);
//		//navigatingCompletedEventHandlerRegistrationToken = dialogWebBrowser().NavigationCompleted(eventHandlerCompleted);
//		_popup.IsOpen(true);
//
//		//dialogWebBrowser().Navigate(dialogUrl);
//		//dialogWebBrowser().Focus(Windows::UI::Xaml::FocusState::Programmatic);
//
//		co_await resume_on_signal(_dialogEventHandle);
//
//		co_return _dialogResponse;
//	}
//
//	winrt::hstring FBDialog::GetRedirectUriString(
//		winrt::hstring DialogName
//	) {
//		auto session = FBSession::ActiveSession();
//		std::wstringstream resultStream;
//		resultStream << session.WebViewRedirectDomain().c_str() << session.WebViewRedirectPath().c_str();
//		hstring result(Uri::EscapeComponent(resultStream.str().c_str()));
//		OutputDebugString(result.c_str());
//		return result;
//	}
//
//	bool FBDialog::IsMobilePlatform() {
//#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
//		return TRUE;
//#else
//		return FALSE;
//#endif
//	}
//
//	Windows::Foundation::Uri FBDialog::BuildLoginDialogUrl(
//		Windows::Foundation::Collections::PropertySet const& parameters
//	) {
//		auto session = FBSession::ActiveSession();
//		std::wstringstream uriString;
//		std::wstringstream apiVersion;
//		apiVersion << L"";
//
//		if (session.APIMajorVersion())
//		{
//			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
//		}
//		uriString << FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/oauth?client_id=" << session.FBAppId().c_str();
//
//		// Use some reasonable default login parameters
//		hstring scope(DefaultScope);
//		hstring displayType(DefaultDisplay);
//		hstring responseType(DefaultResponse);
//
//		uriString << L"&redirect_uri=" << GetRedirectUriString(L"login").c_str();
//
//		for (auto const& parameter : parameters)
//		{
//			hstring key(parameter.Key());
//			hstring value = unbox_value<hstring>(parameter.Value());
//			if (!value.empty()) {
//				if (compare_ordinal(key.c_str(), ScopeKey) == 0) {
//					scope = value;
//				}
//				else if (compare_ordinal(key.c_str(), DisplayKey) == 0) {
//					displayType = value;
//				}
//				else if (compare_ordinal(key.c_str(), ResponseTypeKey) == 0) {
//					responseType = value;
//				}
//				else {
//					uriString << "&" << key.c_str() << "=" << value.c_str();
//				}
//			}
//		}
//
//		uriString << "&" << ScopeKey << "=" << scope.c_str()
//			<< "&" << DisplayKey << "=" << displayType.c_str()
//			<< "&" << ResponseTypeKey << "=" << responseType.c_str();
//
//		return Uri(uriString.str().c_str());
//	}
//
//	Windows::Foundation::Uri FBDialog::BuildFeedDialogUrl(
//		Windows::Foundation::Collections::PropertySet const& parameters
//	) {
//		auto session = FBSession::ActiveSession();
//		std::wstringstream apiVersion;
//		if (session.APIMajorVersion())
//		{
//			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
//		}
//
//		std::wstringstream dialogUriString;
//		dialogUriString <<
//			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/feed?access_token=" <<
//			session.AccessTokenData().AccessToken().c_str() <<
//			L"&redirect_uri=" << GetRedirectUriString(L"feed").c_str() <<
//			L"&display=popup" <<
//			L"&app_id=" << session.FBAppId().c_str();
//
//		//hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
//		//if (queryString.size() > 0)
//		//{
//		//	dialogUriString << "&" << queryString.c_str();
//		//}
//
//		return Uri(dialogUriString.str().c_str());
//	}
//
//	Windows::Foundation::Uri FBDialog::BuildRequestsDialogUrl(
//		Windows::Foundation::Collections::PropertySet const& parameters
//	) {
//		auto session = FBSession::ActiveSession();
//		std::wstringstream apiVersion;
//		if (session.APIMajorVersion())
//		{
//			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
//		}
//
//		std::wstringstream dialogUriString;
//		dialogUriString <<
//			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/apprequests?access_token=" <<
//			session.AccessTokenData().AccessToken().c_str() <<
//			L"&redirect_uri=" << GetRedirectUriString(L"requests").c_str() <<
//			L"&display=popup" <<
//			L"&app_id=" << session.FBAppId().c_str();
//
//		//hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
//		//if (queryString.size() > 0)
//		//{
//		//	dialogUriString << "&" << queryString.c_str();
//		//}
//
//		return Uri(dialogUriString.str().c_str());
//	}
//
//	Windows::Foundation::Uri FBDialog::BuildSendDialogUrl(
//		Windows::Foundation::Collections::PropertySet const& parameters
//	) {
//		auto session = FBSession::ActiveSession();
//		std::wstringstream apiVersion;
//		if (session.APIMajorVersion())
//		{
//			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
//		}
//
//		std::wstringstream dialogUriString;
//		dialogUriString <<
//			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/send?access_token=" <<
//			session.AccessTokenData().AccessToken().c_str() <<
//			L"&redirect_uri=" << GetRedirectUriString(L"send").c_str() <<
//			L"&display=popup" <<
//			L"&app_id=" << session.FBAppId().c_str();
//
//		//hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
//		//if (queryString.size() > 0)
//		//{
//		//	dialogUriString << "&" << queryString.c_str();
//		//}
//
//		return Uri(dialogUriString.str().c_str());
//	}
//
//	void FBDialog::dialogWebView_LoginNavStarting(
//		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
//		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
//	) {
//		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
//		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());
//
//		if (IsLoginSuccessRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			auto tokenData = FBAccessTokenData::FromUri(e.Uri());
//			if (tokenData.has_value())
//			{
//				SetDialogResponse(FBResult(&tokenData));
//			}
//			else
//			{
//				auto err = FBError::FromJson(hstring(ErrorObjectJson));
//				SetDialogResponse(FBResult(err));
//			}
//		}
//		else if (IsDialogCloseRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJson));
//			SetDialogResponse(FBResult(err));
//		}
//	}
//
//	void FBDialog::dialogWebView_FeedNavStarting(
//		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
//		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
//	) {
//		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
//		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());
//
//		if (IsLoginSuccessRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			DebugPrintLine(hstring(L"Feed response is ") + e.Uri().DisplayUri());
//
//			//auto request = FBFeedRequest::FromFeedDialogResponse(e.Uri());
//			//if (request)
//			//{
//			//	SetDialogResponse(make<FBResult>(request));
//			//}
//			//else
//			//{
//			//	auto err = FBError::FromJson(hstring(ErrorObjectJson));
//			//	SetDialogResponse(make<FBResult>(err));
//			//}
//		}
//		else if (IsLogoutRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			DebugPrintLine(hstring(L"Feed response is ") + e.Uri().DisplayUri());
//			auto session = FBSession::ActiveSession();
//			session.LogoutAsync();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
//			SetDialogResponse(FBResult(err));
//		}
//		else if (IsDialogCloseRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJson));
//			SetDialogResponse(FBResult(err));
//		}
//	}
//
//	void FBDialog::dialogWebView_RequestNavStarting(
//		Windows::UI::Xaml::Controls::WebView const /*sender*/,
//		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
//	) {
//		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
//		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());
//
//		if (IsLoginSuccessRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
//
//			auto request = FBAppRequest::FromRequestDialogResponse(e.Uri());
//			//if (request)
//			//{
//			//	SetDialogResponse(make<FBResult>(request));
//			//}
//			//else
//			//{
//			//	auto err = FBError::FromJson(hstring(ErrorObjectJson));
//			//	SetDialogResponse(make<FBResult>(err));
//			//}
//		}
//		else if (IsLogoutRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
//			auto session = FBSession::ActiveSession();
//			session.LogoutAsync();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
//			SetDialogResponse(FBResult(err));
//		}
//		else if (IsDialogCloseRedirect(e.Uri()))
//		{
//			UninitDialog();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJson));
//			SetDialogResponse(FBResult(err));
//		}
//	}
//
//	void FBDialog::dialogWebView_SendNavStarting(
//		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
//		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
//	) {
//		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
//		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());
//
//		//if (IsLoginSuccessRedirect(e.Uri()))
//		//{
//		//	dialogWebBrowser().Stop();
//
//		//	UninitDialog();
//
//		//	DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
//		//	SetDialogResponse(make<FBResult>(FBSendRequest()));
//		//}
//		//else if (IsLogoutRedirect(e.Uri()))
//		//{
//		//	UninitDialog();
//
//		//	DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
//		//	auto session = FBSession::ActiveSession();
//		//	session.LogoutAsync();
//
//		//	auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
//		//	SetDialogResponse(make<FBResult>(err));
//		//}
//		//else if (IsDialogCloseRedirect(e.Uri()))
//		//{
//		//	UninitDialog();
//
//		//	auto err = FBError::FromJson(hstring(ErrorObjectJson));
//		//	SetDialogResponse(make<FBResult>(err));
//		//}
//	}
//
//	void FBDialog::dialogWebView_NavCompleted(
//		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
//		Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
//	) {
//		if (!e.IsSuccess())
//		{
//			UninitDialog();
//
//			auto err = FBError::FromJson(hstring(ErrorObjectJsonNoInternet));
//			SetDialogResponse(FBResult(err));
//		}
//	}
//
//	void FBDialog::CloseDialogButton_OnClick(
//		Windows::Foundation::IInspectable const& /*sender*/,
//		Windows::UI::Xaml::RoutedEventArgs const& /*e*/
//	) {
//		UninitDialog();
//
//		auto err = FBError::FromJson(hstring(ErrorObjectJson));
//		SetDialogResponse(FBResult(err));
//	}
//
//	bool FBDialog::IsLoginSuccessRedirect(
//		Windows::Foundation::Uri const&  response
//	) {
//		auto session = FBSession::ActiveSession();
//		return (compare_ordinal(response.Path().c_str(), session.WebViewRedirectPath().c_str()) == 0);
//	}
//
//	bool FBDialog::IsLogoutRedirect(
//		Windows::Foundation::Uri const& response
//	) {
//		return (compare_ordinal(response.Path().c_str(), FACEBOOK_LOGOUT_PATH) == 0);
//	}
//
//	bool FBDialog::IsDialogCloseRedirect(
//		Windows::Foundation::Uri const&  response
//	) {
//		return (compare_ordinal(response.Path().c_str(), FACEBOOK_DIALOG_CLOSE_PATH) == 0);
//	}
//
//	void FBDialog::OnSizeChanged(
//		Windows::UI::Core::CoreWindow const& sender,
//		Windows::UI::Core::WindowSizeChangedEventArgs const& /*args*/
//	) {
//		//Height(sender.Bounds().Height);
//		//Width(sender.Bounds().Width);
//	}
//
//	void FBDialog::SetDialogResponse(winsdkfb::FBResult dialogResponse) {
//		if (_dialogEventHandle != nullptr) {
//			_dialogResponse = dialogResponse;
//			SetEvent(_dialogEventHandle);
//		}
//		_dialogEventHandle = nullptr;
//	}
}
