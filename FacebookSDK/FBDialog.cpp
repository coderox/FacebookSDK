#include "FBDialog.h"
#include "FBSession.h"
#include "FBResult.h"
#include "FBFeedRequest.h"
#include "FacebookAppRequest.h"
#include "FBSendRequest.h"
#include "HttpManager.h"
#include "Utilities.h"
#include "FBConstants.h"

#include <sstream>
#include <string>

#include <winrt/Windows.Web.Http.Filters.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Xaml.Markup.h>

using namespace std;
using namespace winrt;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;
using namespace Windows::UI;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Markup;

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
	hstring const output(msg + L"\n");
	OutputDebugString(output.c_str());
}
#else
#define DebugPrintLine(msg) ((void) 0)
#endif

namespace winsdkfb
{
	FBDialog::FBDialog()
	{
		InitializeComponent();
		InitDialog();
	}

	void FBDialog::InitializeComponent() {
		if (contentLoaded)
			return;

		contentLoaded = true;

		SolidColorBrush backgroundBrush(Color({ 0x7f, 0x00, 0x00, 0x00 }));
		Grid layoutRoot;
		layoutRoot.Background(backgroundBrush);

		dialogWebBrowser.Margin(Thickness{ 30,30,30,30 });

		closeDialogButton.HorizontalAlignment(HorizontalAlignment::Right);
		closeDialogButton.VerticalAlignment(VerticalAlignment::Top);
		closeDialogButton.Width(30);
		closeDialogButton.Height(30);
		closeDialogButton.Margin(Thickness{ 0,0,0,5 });

		closeDialogButton.Click({ get_strong(), &FBDialog::CloseDialogButton_OnClick });
		const auto styleText = L"<Style xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation' TargetType='Button'> \
			<Setter Property='Background' Value='Transparent'/> \
			<Setter Property='Template'> \
			<Setter.Value> \
			<ControlTemplate TargetType='Button'> \
			<Grid> \
			<Rectangle Fill='#FFF4F4F5' Opacity='0'/> \
			<Path Data='M2.55469,0.445312 L24,21.8672 L45.4453,0.445312 L47.5547,2.55469 L26.1328,24 L47.5547,45.4453 L45.4453,47.5547 L24,26.1328 L2.55469,47.5547 L0.445312,45.4453 L21.8672,24 L0.445312,2.55469 z' Fill='{ThemeResource ButtonForegroundThemeBrush}' Margin='0' RenderTransformOrigin='0.5,0.5' Stretch='Fill' UseLayoutRounding='False' /> \
			</Grid></ControlTemplate></Setter.Value></Setter></Style>";
		auto style = XamlReader::Load(styleText).as<winrt::Windows::UI::Xaml::Style>();
		closeDialogButton.Style(style);

		Grid grid;
		grid.Margin({ 20, 40 });
		grid.MaxWidth(650);
		grid.MaxHeight(600);
		grid.Children().Append(closeDialogButton);
		grid.Children().Append(dialogWebBrowser);
		layoutRoot.Children().Append(grid);

		Content(layoutRoot);
	}

	void FBDialog::InitDialog()
	{
		popup = nullptr;

		auto coreWindow = CoreApplication::MainView().CoreWindow();

		popup = Popup();

		popup.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Stretch);
		popup.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Stretch);

		Margin(Thickness({ 0, 0, 0, 0 }));
		Height(coreWindow.Bounds().Height);
		Width(coreWindow.Bounds().Width);

		sizeChangedEventRegistrationToken = coreWindow.SizeChanged(
			TypedEventHandler<CoreWindow, WindowSizeChangedEventArgs>
			(this, &FBDialog::OnSizeChanged));

		dialogEventHandle = CreateEvent(nullptr, true, false, nullptr);

		popup.Child(*this);
	}

	void FBDialog::UnInitDialog() const {
		dialogWebBrowser.Stop();
		dialogWebBrowser.NavigationStarting(navigatingStartingEventHandlerRegistrationToken);
		dialogWebBrowser.NavigationCompleted(navigatingCompletedEventHandlerRegistrationToken);
		CoreApplication::MainView().CoreWindow().SizeChanged(sizeChangedEventRegistrationToken);

		popup.IsOpen(false);

		//
		// This breaks the circular dependency between the popup and dialog
		// class, and is essential in order for the dialog to be disposed of
		// properly.
		//
		popup.Child(nullptr);
	}

	concurrency::task<winsdkfb::FBResult> FBDialog::ShowLoginDialogAsync(unordered_map<wstring, wstring> const parameters)
	{
		const auto handlerStarting = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationStartingEventArgs>(
			this, &FBDialog::dialogWebView_LoginNavStarting);
		const auto handlerCompleted = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationCompletedEventArgs>(
			this, &FBDialog::dialogWebView_NavCompleted);

		const winsdkfb::DialogUriBuilder uriBuilder = [this](unordered_map<wstring, wstring> parameters) -> Windows::Foundation::Uri {
			return this->BuildLoginDialogUrl(parameters);
		};

		return ShowDialog(uriBuilder, handlerStarting, handlerCompleted, parameters);
	}

	concurrency::task<winsdkfb::FBResult> FBDialog::ShowFeedDialogAsync(unordered_map<wstring, wstring> const parameters)
	{
		const auto handlerStarting = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationStartingEventArgs>(
			this, &FBDialog::dialogWebView_FeedNavStarting);
		const auto handlerCompleted = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationCompletedEventArgs>(
			this, &FBDialog::dialogWebView_NavCompleted);

		const winsdkfb::DialogUriBuilder uriBuilder = [this](unordered_map<wstring, wstring> parameters) -> Windows::Foundation::Uri {
			return this->BuildFeedDialogUrl(parameters);
		};

		return ShowDialog(uriBuilder, handlerStarting, handlerCompleted, parameters);
	}

	concurrency::task<winsdkfb::FBResult> FBDialog::ShowRequestsDialogAsync(unordered_map<wstring, wstring> const parameters)
	{
		const auto handlerStarting = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationStartingEventArgs>(
			this, &FBDialog::dialogWebView_RequestNavStarting);
		const auto handlerCompleted = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationCompletedEventArgs>(
			this, &FBDialog::dialogWebView_NavCompleted);

		const winsdkfb::DialogUriBuilder uriBuilder = [this](unordered_map<wstring, wstring> parameters) -> Windows::Foundation::Uri {
			return this->BuildRequestsDialogUrl(parameters);
		};

		return ShowDialog(uriBuilder, handlerStarting, handlerCompleted, parameters);
	}

	concurrency::task<winsdkfb::FBResult> FBDialog::ShowSendDialogAsync(unordered_map<wstring, wstring> const parameters)
	{
		const auto handlerStarting = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationStartingEventArgs>(
			this, &FBDialog::dialogWebView_SendNavStarting);
		const auto handlerCompleted = TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationCompletedEventArgs>(
			this, &FBDialog::dialogWebView_NavCompleted);

		const winsdkfb::DialogUriBuilder uriBuilder = [this](unordered_map<wstring, wstring> parameters) -> Windows::Foundation::Uri {
			return this->BuildSendDialogUrl(parameters);
		};

		return ShowDialog(uriBuilder, handlerStarting, handlerCompleted, parameters);
	}

	wstring FBDialog::GetFBServerUrl()
	{
		return FBDialog::IsMobilePlatform() ? FACEBOOK_MOBILE_SERVER_NAME : FACEBOOK_DESKTOP_SERVER_NAME;
	}

	void FBDialog::DeleteCookies()
	{
		const HttpBaseProtocolFilter filter;
		const HttpCookieManager cookieManager(filter.CookieManager());
		HttpCookieCollection cookiesJar(cookieManager.GetCookies(Uri(FBDialog::GetFBServerUrl())));
		for (auto const& cookie : cookiesJar)
		{
			cookieManager.DeleteCookie(cookie);
		}
	}

	concurrency::task<winsdkfb::FBResult> FBDialog::ShowDialog(
		DialogUriBuilder const& uriBuilder,
		TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationStartingEventArgs> const& eventHandlerStarting,
		TypedEventHandler<winrt::Windows::UI::Xaml::Controls::WebView, WebViewNavigationCompletedEventArgs > const& eventHandlerCompleted,
		unordered_map<wstring, wstring> const parameters
	) {
		const auto dialogUrl = uriBuilder(parameters);

		navigatingStartingEventHandlerRegistrationToken = dialogWebBrowser.NavigationStarting(eventHandlerStarting);
		navigatingCompletedEventHandlerRegistrationToken = dialogWebBrowser.NavigationCompleted(eventHandlerCompleted);
		popup.IsOpen(true);

		dialogWebBrowser.Navigate(dialogUrl);
		dialogWebBrowser.Focus(FocusState::Programmatic);

		co_await resume_on_signal(dialogEventHandle);

		co_return _dialogResponse;
	}

	std::wstring FBDialog::GetRedirectUriString(
		std::wstring DialogName
	) {
		const auto session = FBSession::ActiveSession();
		std::wstringstream resultStream;
		resultStream << session->WebViewRedirectDomain().c_str() << session->WebViewRedirectPath().c_str();
		wstring result(Uri::EscapeComponent(resultStream.str().c_str()));
		OutputDebugString(result.c_str());
		return result;
	}

	bool FBDialog::IsMobilePlatform() {
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
		return TRUE;
#else
		return FALSE;
#endif
	}

	Windows::Foundation::Uri FBDialog::BuildLoginDialogUrl(
		unordered_map<wstring, wstring> const parameters
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream uriString;
		std::wstringstream apiVersion;
		apiVersion << L"";

		if (session->APIMajorVersion())
		{
			apiVersion << L"/v" << session->APIMajorVersion() << L"." << session->APIMinorVersion() << L"/";
		}
		uriString << FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/oauth?client_id=" << session->FBAppId().c_str();

		// Use some reasonable default login parameters
		hstring scope(DefaultScope);
		hstring displayType(DefaultDisplay);
		hstring responseType(DefaultResponse);

		uriString << L"&redirect_uri=" << GetRedirectUriString(L"login").c_str();

		for (auto const& parameter : parameters)
		{
			auto key = parameter.first;
			auto value = parameter.second;
			if (!value.empty()) {
				if (compare_ordinal(key.c_str(), ScopeKey) == 0) {
					scope = value;
				}
				else if (compare_ordinal(key.c_str(), DisplayKey) == 0) {
					displayType = value;
				}
				else if (compare_ordinal(key.c_str(), ResponseTypeKey) == 0) {
					responseType = value;
				}
				else {
					uriString << "&" << key.c_str() << "=" << value.c_str();
				}
			}
		}

		uriString << "&" << ScopeKey << "=" << scope.c_str()
			<< "&" << DisplayKey << "=" << displayType.c_str()
			<< "&" << ResponseTypeKey << "=" << responseType.c_str();

		return Uri(uriString.str().c_str());
	}

	Windows::Foundation::Uri FBDialog::BuildFeedDialogUrl(
		unordered_map<wstring, wstring> const parameters
	) {
		const auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session->APIMajorVersion())
		{
			apiVersion << L"/v" << session->APIMajorVersion() << L"." << session->APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/feed?access_token=" <<
			session->AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"feed").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session->FBAppId().c_str();

		const hstring queryString(HttpManager::Instance()->ParametersToQueryString(parameters));
		if (queryString.size() > 0)
		{
			dialogUriString << "&" << queryString.c_str();
		}

		return Uri(dialogUriString.str().c_str());
	}

	Windows::Foundation::Uri FBDialog::BuildRequestsDialogUrl(
		unordered_map<wstring, wstring> const parameters
	) {
		const auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session->APIMajorVersion())
		{
			apiVersion << L"/v" << session->APIMajorVersion() << L"." << session->APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/apprequests?access_token=" <<
			session->AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"requests").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session->FBAppId().c_str();

		const hstring queryString(HttpManager::Instance()->ParametersToQueryString(parameters));
		if (queryString.size() > 0)
		{
			dialogUriString << "&" << queryString.c_str();
		}

		return Uri(dialogUriString.str().c_str());
	}

	Windows::Foundation::Uri FBDialog::BuildSendDialogUrl(
		unordered_map<wstring, wstring> const parameters
	) {
		const auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session->APIMajorVersion())
		{
			apiVersion << L"/v" << session->APIMajorVersion() << L"." << session->APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/send?access_token=" <<
			session->AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"send").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session->FBAppId().c_str();

		const hstring queryString(HttpManager::Instance()->ParametersToQueryString(parameters));
		if (queryString.size() > 0)
		{
			dialogUriString << "&" << queryString.c_str();
		}

		return Uri(dialogUriString.str().c_str());
	}

	void FBDialog::dialogWebView_LoginNavStarting(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(wstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(wstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UnInitDialog();

			auto tokenData = FBAccessTokenData::FromUri(e.Uri());
			if (tokenData.has_value())
			{
				SetDialogResponse(FBResult(std::any_cast<FBAccessTokenData>(tokenData)));
			}
			else
			{
				auto err = FBError::FromJson(wstring(ErrorObjectJson));
				SetDialogResponse(FBResult(err));
			}
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UnInitDialog();

			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			SetDialogResponse(FBResult(err));
		}
	}

	void FBDialog::dialogWebView_FeedNavStarting(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(wstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(wstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UnInitDialog();

			DebugPrintLine(wstring(L"Feed response is ") + e.Uri().DisplayUri());

			try {
				auto request = FBFeedRequest::FromFeedDialogResponse(e.Uri());
				SetDialogResponse(FBResult(request));
			} catch(...) {
				auto err = FBError::FromJson(wstring(ErrorObjectJson));
				SetDialogResponse(FBResult(err));
			}
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UnInitDialog();

			DebugPrintLine(hstring(L"Feed response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session->LogoutAsync();

			auto err = FBError::FromJson(wstring(ErrorObjectJsonLogout));
			SetDialogResponse(FBResult(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UnInitDialog();

			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			SetDialogResponse(FBResult(err));
		}
	}

	void FBDialog::dialogWebView_RequestNavStarting(
		Windows::UI::Xaml::Controls::WebView const /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(wstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(wstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UnInitDialog();

			DebugPrintLine(wstring(L"Request response is ") + e.Uri().DisplayUri());

			try {
				auto request = FacebookAppRequest::FromRequestDialogResponse(e.Uri());
				SetDialogResponse(FBResult(request));
			}
			catch (...) {
				auto err = FBError::FromJson(wstring(ErrorObjectJson));
				SetDialogResponse(FBResult(err));
			}
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UnInitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session->LogoutAsync();

			auto err = FBError::FromJson(wstring(ErrorObjectJsonLogout));
			SetDialogResponse(FBResult(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UnInitDialog();

			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			SetDialogResponse(FBResult(err));
		}
	}

	void FBDialog::dialogWebView_SendNavStarting(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			dialogWebBrowser.Stop();

			UnInitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			SetDialogResponse(FBResult(FBSendRequest()));
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UnInitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session->LogoutAsync();

			auto err = FBError::FromJson(wstring(ErrorObjectJsonLogout));
			SetDialogResponse(FBResult(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UnInitDialog();

			auto err = FBError::FromJson(wstring(ErrorObjectJson));
			SetDialogResponse(FBResult(err));
		}
	}

	void FBDialog::dialogWebView_NavCompleted(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
	) {
		if (!e.IsSuccess())
		{
			UnInitDialog();

			auto err = FBError::FromJson(wstring(ErrorObjectJsonNoInternet));
			SetDialogResponse(FBResult(err));
		}
	}

	void FBDialog::CloseDialogButton_OnClick(
		Windows::Foundation::IInspectable const& /*sender*/,
		Windows::UI::Xaml::RoutedEventArgs const& /*e*/
	) {
		UnInitDialog();

		auto err = FBError::FromJson(wstring(ErrorObjectJson));
		SetDialogResponse(FBResult(err));
	}

	bool FBDialog::IsLoginSuccessRedirect(
		Windows::Foundation::Uri const&  response
	) {
		const auto session = FBSession::ActiveSession();
		return (compare_ordinal(response.Path().c_str(), session->WebViewRedirectPath().c_str()) == 0);
	}

	bool FBDialog::IsLogoutRedirect(
		Windows::Foundation::Uri const& response
	) {
		return (compare_ordinal(response.Path().c_str(), FACEBOOK_LOGOUT_PATH) == 0);
	}

	bool FBDialog::IsDialogCloseRedirect(
		Windows::Foundation::Uri const&  response
	) {
		return (compare_ordinal(response.Path().c_str(), FACEBOOK_DIALOG_CLOSE_PATH) == 0);
	}

	void FBDialog::OnSizeChanged(
		Windows::UI::Core::CoreWindow const& sender,
		Windows::UI::Core::WindowSizeChangedEventArgs const& /*args*/
	) const {
		Height(sender.Bounds().Height);
		Width(sender.Bounds().Width);
	}

	void FBDialog::SetDialogResponse(winsdkfb::FBResult dialogResponse) {
		if (dialogEventHandle != nullptr) {
			_dialogResponse = dialogResponse;
			SetEvent(dialogEventHandle);
		}
		dialogEventHandle = nullptr;
	}
}
