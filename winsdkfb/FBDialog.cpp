﻿#include "pch.h"
#include "FBDialog.h"
#include "FBSession.h"
#include "FBResult.h"
#include "FBFeedRequest.h"
#include "FBAppRequest.h"
#include "HttpManager.h"
#include <sstream>
#include <string>
#include <Utilities.h>

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

namespace winrt::winsdkfb::implementation
{
	FBDialog::FBDialog()
	{
		InitializeComponent();
		InitDialog();
	}

	void FBDialog::InitDialog()
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
			(this, &FBDialog::OnSizeChanged));

		_dialogEventHandle = CreateEvent(nullptr, true, false, nullptr);

		_popup.Child(*this);
	}

	void FBDialog::UninitDialog()
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

	IAsyncOperation<winsdkfb::FBResult> FBDialog::ShowLoginDialogAsync(PropertySet const parameters)
	{
		auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_LoginNavStarting);
		auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildLoginDialogUrl), handlerStarting, handlerCompleted, parameters);
	}

	IAsyncOperation<winsdkfb::FBResult> FBDialog::ShowFeedDialogAsync(PropertySet const parameters)
	{
		auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_FeedNavStarting);
		auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildFeedDialogUrl), handlerStarting, handlerCompleted, parameters);
	}

	IAsyncOperation<winsdkfb::FBResult> FBDialog::ShowRequestsDialogAsync(PropertySet const parameters)
	{
		auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_RequestNavStarting);
		auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildRequestsDialogUrl), handlerStarting, handlerCompleted, parameters);
	}

	IAsyncOperation<winsdkfb::FBResult> FBDialog::ShowSendDialogAsync(PropertySet const parameters)
	{
		auto handlerStarting = TypedEventHandler<WebView, WebViewNavigationStartingEventArgs>(this, &FBDialog::dialogWebView_SendNavStarting);
		auto handlerCompleted = TypedEventHandler<WebView, WebViewNavigationCompletedEventArgs>(this, &FBDialog::dialogWebView_NavCompleted);
		return ShowDialog(DialogUriBuilder(this, &FBDialog::BuildSendDialogUrl), handlerStarting, handlerCompleted, parameters);
	}

	hstring FBDialog::GetFBServerUrl()
	{
		return FBDialog::IsMobilePlatform() ? FACEBOOK_MOBILE_SERVER_NAME : FACEBOOK_DESKTOP_SERVER_NAME;
	}

	void FBDialog::DeleteCookies()
	{
		HttpBaseProtocolFilter filter;
		HttpCookieManager cookieManager(filter.CookieManager());
		HttpCookieCollection cookiesJar(cookieManager.GetCookies(Uri(FBDialog::GetFBServerUrl())));
		for (auto const& cookie : cookiesJar)
		{
			cookieManager.DeleteCookie(cookie);
		}
	}

	IAsyncOperation<winsdkfb::FBResult> FBDialog::ShowDialog(
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

		co_await resume_on_signal(_dialogEventHandle);

		co_return _dialogResponse;
	}

	winrt::hstring FBDialog::GetRedirectUriString(
		winrt::hstring DialogName
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream resultStream;
		resultStream << session.WebViewRedirectDomain().c_str() << session.WebViewRedirectPath().c_str();
		hstring result(Uri::EscapeComponent(resultStream.str().c_str()));
		OutputDebugString(result.c_str());
		return result;
	}

	BOOL FBDialog::IsMobilePlatform() {
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
		return TRUE;
#else
		return FALSE;
#endif
	}

	Windows::Foundation::Uri FBDialog::BuildLoginDialogUrl(
		Windows::Foundation::Collections::PropertySet const& parameters
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream uriString;
		std::wstringstream apiVersion;
		apiVersion << L"";

		if (session.APIMajorVersion())
		{
			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
		}
		uriString << FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/oauth?client_id=" << session.FBAppId().c_str();

		// Use some reasonable default login parameters
		hstring scope(DefaultScope);
		hstring displayType(DefaultDisplay);
		hstring responseType(DefaultResponse);

		uriString << L"&redirect_uri=" << GetRedirectUriString(L"login").c_str();

		for (auto const& parameter : parameters)
		{
			hstring key(parameter.Key());
			hstring value = unbox_value<hstring>(parameter.Value());
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

		return uriString.str().c_str();
	}

	Windows::Foundation::Uri FBDialog::BuildFeedDialogUrl(
		Windows::Foundation::Collections::PropertySet const& parameters
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session.APIMajorVersion())
		{
			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/feed?access_token=" <<
			session.AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"feed").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session.FBAppId().c_str();

		hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
		if (queryString.size() > 0)
		{
			dialogUriString << "&" << queryString.c_str();
		}

		return Uri(dialogUriString.str().c_str());
	}

	Windows::Foundation::Uri FBDialog::BuildRequestsDialogUrl(
		Windows::Foundation::Collections::PropertySet const& parameters
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session.APIMajorVersion())
		{
			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/apprequests?access_token=" <<
			session.AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"requests").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session.FBAppId().c_str();

		hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
		if (queryString.size() > 0)
		{
			dialogUriString << "&" << queryString.c_str();
		}

		return Uri(dialogUriString.str().c_str());
	}

	Windows::Foundation::Uri FBDialog::BuildSendDialogUrl(
		Windows::Foundation::Collections::PropertySet const& parameters
	) {
		auto session = FBSession::ActiveSession();
		std::wstringstream apiVersion;
		if (session.APIMajorVersion())
		{
			apiVersion << L"/v" << session.APIMajorVersion() << L"." << session.APIMinorVersion() << L"/";
		}

		std::wstringstream dialogUriString;
		dialogUriString <<
			FBDialog::GetFBServerUrl().c_str() << apiVersion.str() << L"dialog/send?access_token=" <<
			session.AccessTokenData().AccessToken().c_str() <<
			L"&redirect_uri=" << GetRedirectUriString(L"send").c_str() <<
			L"&display=popup" <<
			L"&app_id=" << session.FBAppId().c_str();

		hstring queryString = HttpManager::Instance().ParametersToQueryString(parameters.GetView());
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
		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UninitDialog();

			auto tokenData = FBAccessTokenData::FromUri(e.Uri());
			if (tokenData)
			{
				SetDialogResponse(make<FBResult>(tokenData));
			}
			else
			{
				auto err = FBError::FromJson(hstring(ErrorObjectJson));
				SetDialogResponse(make<FBResult>(err));
			}
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UninitDialog();

			auto err = FBError::FromJson(hstring(ErrorObjectJson));
			SetDialogResponse(make<FBResult>(err));
		}
	}

	void FBDialog::dialogWebView_FeedNavStarting(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UninitDialog();

			DebugPrintLine(hstring(L"Feed response is ") + e.Uri().DisplayUri());

			auto request = FBFeedRequest::FromFeedDialogResponse(e.Uri());
			if (request)
			{
				SetDialogResponse(make<FBResult>(request));
			}
			else
			{
				auto err = FBError::FromJson(hstring(ErrorObjectJson));
				SetDialogResponse(make<FBResult>(err));
			}
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UninitDialog();

			DebugPrintLine(hstring(L"Feed response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session.LogoutAsync();

			auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
			SetDialogResponse(make<FBResult>(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UninitDialog();

			auto err = FBError::FromJson(hstring(ErrorObjectJson));
			SetDialogResponse(make<FBResult>(err));
		}
	}

	void FBDialog::dialogWebView_RequestNavStarting(
		Windows::UI::Xaml::Controls::WebView const /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs const& e
	) {
		DebugPrintLine(hstring(L"Navigating to ") + e.Uri().DisplayUri());
		DebugPrintLine(hstring(L"Path is ") + e.Uri().Path());

		if (IsLoginSuccessRedirect(e.Uri()))
		{
			UninitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());

			auto request = FBAppRequest::FromRequestDialogResponse(e.Uri());
			if (request)
			{
				SetDialogResponse(make<FBResult>(request));
			}
			else
			{
				auto err = FBError::FromJson(hstring(ErrorObjectJson));
				SetDialogResponse(make<FBResult>(err));
			}
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UninitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session.LogoutAsync();

			auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
			SetDialogResponse(make<FBResult>(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UninitDialog();

			auto err = FBError::FromJson(hstring(ErrorObjectJson));
			SetDialogResponse(make<FBResult>(err));
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
			dialogWebBrowser().Stop();

			UninitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			SetDialogResponse(make<FBResult>(FBSendRequest()));
		}
		else if (IsLogoutRedirect(e.Uri()))
		{
			UninitDialog();

			DebugPrintLine(hstring(L"Request response is ") + e.Uri().DisplayUri());
			auto session = FBSession::ActiveSession();
			session.LogoutAsync();

			auto err = FBError::FromJson(hstring(ErrorObjectJsonLogout));
			SetDialogResponse(make<FBResult>(err));
		}
		else if (IsDialogCloseRedirect(e.Uri()))
		{
			UninitDialog();

			auto err = FBError::FromJson(hstring(ErrorObjectJson));
			SetDialogResponse(make<FBResult>(err));
		}
	}

	void FBDialog::dialogWebView_NavCompleted(
		Windows::UI::Xaml::Controls::WebView const& /*sender*/,
		Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs const& e
	) {
		if (!e.IsSuccess())
		{
			UninitDialog();

			auto err = FBError::FromJson(hstring(ErrorObjectJsonNoInternet));
			SetDialogResponse(make<FBResult>(err));
		}
	}

	void FBDialog::CloseDialogButton_OnClick(
		Windows::Foundation::IInspectable const& /*sender*/,
		Windows::UI::Xaml::RoutedEventArgs const& /*e*/
	) {
		UninitDialog();

		auto err = FBError::FromJson(hstring(ErrorObjectJson));
		SetDialogResponse(make<FBResult>(err));
	}

	bool FBDialog::IsLoginSuccessRedirect(
		Windows::Foundation::Uri const&  response
	) {
		auto session = FBSession::ActiveSession();
		return (compare_ordinal(response.Path().c_str(), session.WebViewRedirectPath().c_str()) == 0);
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
	) {
		Height(sender.Bounds().Height);
		Width(sender.Bounds().Width);
	}

	void FBDialog::SetDialogResponse(winsdkfb::FBResult dialogResponse) {
		if (_dialogEventHandle != nullptr) {
			_dialogResponse = dialogResponse;
			SetEvent(_dialogEventHandle);
		}
		_dialogEventHandle = nullptr;
	}
}
