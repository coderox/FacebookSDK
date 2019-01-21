#include "pch.h"
#include "FacebookDialog.h"

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
		throw hresult_not_implemented();
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
}
