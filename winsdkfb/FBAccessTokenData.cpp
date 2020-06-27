#include "pch.h"
#include "FBAccessTokenData.h"
#include "Generated\Graph.FBPermission.h"
#include <algorithm>
#include <string>
#include "Utilities.h"

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;

namespace winrt::winsdkfb::implementation
{
	FBAccessTokenData::FBAccessTokenData(
		std::wstring accessToken,
		std::wstring expiration,
		std::wstring dataAccessExpiration)
		: _accessToken(accessToken)
		, _hasExpirationDate(false)
		, _hasDataAccessExpirationDate(false)
	{
		if (!expiration.empty())
		{
			CalculateExpirationDateTime(expiration);
		}
		if (!dataAccessExpiration.empty()) {
			CalculateDataAccessExpirationDateTime(dataAccessExpiration);
		}
#ifdef _DEBUG
		DebugPrintExpirationTime();
#endif
	}

	FBAccessTokenData::FBAccessTokenData(hstring const& accessToken, DateTime const& expiration)
		: _accessToken(accessToken.c_str())
		, _expirationDate(expiration)
		, _hasExpirationDate(true)
		, _hasDataAccessExpirationDate(false)
	{
#ifdef _DEBUG
		DebugPrintExpirationTime();
#endif
		auto v{ winrt::single_threaded_vector<hstring>() };
		_grantedPermissions = make<FBPermissions>(v.GetView());
		_declinedPermissions = make<FBPermissions>(v.GetView());
	}

	FBAccessTokenData::FBAccessTokenData(hstring const& accessToken, Windows::Foundation::DateTime const& expiration, Windows::Foundation::DateTime const& dataAccessExpiration)
		: _accessToken(accessToken.c_str())
		, _expirationDate(expiration)
		, _dataAccessExpirationDate(dataAccessExpiration)
		, _hasExpirationDate(true)
		, _hasDataAccessExpirationDate(true)
	{
#ifdef _DEBUG
		DebugPrintExpirationTime();
#endif
		auto v{ winrt::single_threaded_vector<hstring>() };
		_grantedPermissions = make<FBPermissions>(v.GetView());
		_declinedPermissions = make<FBPermissions>(v.GetView());
	}

	hstring FBAccessTokenData::AccessToken()
	{
		return _accessToken.c_str();
	}

	Windows::Foundation::DateTime FBAccessTokenData::ExpirationDate()
	{
		return _expirationDate;
	}

	Windows::Foundation::DateTime FBAccessTokenData::DataAccessExpirationDate()
	{
		return _dataAccessExpirationDate;
	}

	winsdkfb::FBPermissions FBAccessTokenData::GrantedPermissions()
	{
		return _grantedPermissions;
	}

	winsdkfb::FBPermissions FBAccessTokenData::DeclinedPermissions()
	{
		return _declinedPermissions;
	}

	bool FBAccessTokenData::IsExpired()
	{
		bool expired = true;
		Calendar cal;
		cal.SetToNow();
		expired = (cal.CompareDateTime(_expirationDate) >= 0);
		return _hasExpirationDate && expired;
	}

	bool FBAccessTokenData::IsDataAccessExpired()
	{
		bool expired = true;
		Calendar cal;
		cal.SetToNow();
		expired = (cal.CompareDateTime(_expirationDate) >= 0);
		return _hasDataAccessExpirationDate && expired;
	}

	bool FBAccessTokenData::HasExpirationDate() {
		return _hasExpirationDate;
	}

	bool FBAccessTokenData::HasDataAccessExpirationDate() {
		return _hasDataAccessExpirationDate;
	}

	void FBAccessTokenData::SetPermissions(Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> const& perms)
	{
		_grantedPermissions = nullptr;
		_declinedPermissions = nullptr;
		IVector<hstring> granted = { winrt::single_threaded_vector<hstring>() };
		IVector<hstring> declined = { winrt::single_threaded_vector<hstring>() };

		for (unsigned int i = 0; i < perms.Size(); i++)
		{
			Graph::FBPermission perm = unbox_value<Graph::FBPermission>(perms.GetAt(i));

			if (compare_ordinal(perm.Status().c_str(), L"granted") == 0)
			{
				granted.Append(perm.Permission());
			}
			else if (compare_ordinal(perm.Status().c_str(), L"declined") == 0)
			{
				declined.Append(perm.Permission());
			}
		}

		_grantedPermissions = make<FBPermissions>(granted.GetView());
		_declinedPermissions = make<FBPermissions>(declined.GetView());
	}

	winsdkfb::FBAccessTokenData FBAccessTokenData::FromUri(Windows::Foundation::Uri const& response)
	{
		bool gotToken = false;
		bool gotExpiration = false;
		bool gotDataAccessExpiration = false;
		hstring token;
		hstring expiration;
		hstring dataAccessExpiration;
		winsdkfb::FBAccessTokenData data{ nullptr };

		WwwFormUrlDecoder decoder = FBAccessTokenData::ParametersFromResponse(response);

		for (unsigned int i = 0; i < decoder.Size(); i++)
		{
			IWwwFormUrlDecoderEntry entry = decoder.GetAt(i);

			if (entry.Name() == L"access_token")
			{
				token = entry.Value();
				gotToken = true;
			}
			else if (entry.Name() == L"expires_in")
			{
				expiration = entry.Value();
				gotExpiration = true;
			}
			else if (entry.Name() == L"data_access_expiration_time")
			{
				dataAccessExpiration = entry.Value();
				gotDataAccessExpiration = true;
			}
		}

		if (gotToken && gotExpiration && gotDataAccessExpiration)
		{
			data = make<FBAccessTokenData>(token.c_str(), expiration.c_str(), dataAccessExpiration.c_str());
		}

		return data;
	}

	void FBAccessTokenData::CalculateExpirationDateTime(
		wstring expiration
	) {
		Calendar cal;
		int numSecs = _wtoi(expiration.c_str());
		if (numSecs != 0) {
			cal.SetToNow();
			cal.AddSeconds(numSecs);
			_expirationDate = cal.GetDateTime();
			_hasExpirationDate = true;
		}
	}

	void FBAccessTokenData::CalculateDataAccessExpirationDateTime(
		wstring expiration
	) {
		Calendar cal;
		int numSecs = _wtoi(expiration.c_str());
		if (numSecs != 0) {
			cal.SetToNow();
			cal.AddSeconds(numSecs);
			_dataAccessExpirationDate = cal.GetDateTime();
			_hasDataAccessExpirationDate = true;
		}
	}

	WwwFormUrlDecoder FBAccessTokenData::ParametersFromResponse(Uri const& response)
	{
		// facebook sometimes returns the access token, etc., as a Uri fragment
		// but in the query string, making it not parse correctly. Here we check
		// if this is the case (look for "?#" pattern in the string and turn it
		// into a normal Uri fragment that we can fix later
		auto responseString = wstring(response.DisplayUri().c_str());
		auto found = responseString.find(L"?#");
		Uri nonConstResponse(response);
		if (found != std::string::npos)
		{
			wstring uriFragment(responseString.substr(found + 1)); // +1 to move past '?' char
			wstring uriDomain(responseString.substr(0, found));
			wstring joinedUri(uriDomain + uriFragment);
			nonConstResponse = Uri(joinedUri.c_str());
		}

		WwwFormUrlDecoder parameters(nonConstResponse.QueryParsed());
		if (!parameters.Size())
		{
			// FB sometimes returns access token, etc., in the Uri fragment
			// rather than the query string.  WinRT only lets you parse a query
			// string from a full Uri, so we'll mock one up with the fragment from
			// the original response as the query string, then parse that.
			//
			// Note that the Uri::Fragment property includes the leading '#'
			// character, inconveniently, so we have to strip this character or
			// we'll just end up with a Uri with the same fragment and an empty
			// query.
			wstring frag(nonConstResponse.Fragment().c_str());

			responseString = L"file:///dev/null?" + hstring(frag.substr(1).c_str());
			OutputDebugString(responseString.c_str());
			OutputDebugString(L"\n");

			Uri newResponse(responseString);
			parameters = newResponse.QueryParsed();
		}
		return parameters;
	}

#ifdef _DEBUG
	void FBAccessTokenData::DebugPrintExpirationTime(
	) {
		DateTimeFormatter dtfDay(YearFormat::Default, MonthFormat::Default, DayFormat::Default, DayOfWeekFormat::Default);
		DateTimeFormatter dtfTime(HourFormat::Default, MinuteFormat::Default, SecondFormat::Default);
		hstring msgString(L"Token expires at " + dtfDay.Format(_expirationDate) + L", " + dtfTime.Format(_expirationDate) + L"\n");
		OutputDebugString(msgString.c_str());
		if (_hasDataAccessExpirationDate) {
			msgString = L"Data Access expires at " + dtfDay.Format(_dataAccessExpirationDate) + L", " + dtfTime.Format(_dataAccessExpirationDate) + L"\n";
			OutputDebugString(msgString.c_str());
		}
		else {
			OutputDebugString(L"Data Access expires never!");
		}
	}
#endif
}
