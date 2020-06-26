#include "FBAccessTokenData.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "Graph/FBPermission.h"
#include <string>
#include <sstream>
#include "Utilities.h"

#include <winrt/Windows.Globalization.DateTimeFormatting.h>

using namespace winrt;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;

namespace winsdkfb {
FBAccessTokenData::FBAccessTokenData(
	wstring accessToken,
	const wstring expiration,
	const wstring dataAccessExpiration)
	: _accessToken(std::move(accessToken)) {
	if (!expiration.empty()) {
		CalculateExpirationDateTime(expiration);
	}

	if (!dataAccessExpiration.empty()) {
		CalculateDataAccessExpirationDateTime(dataAccessExpiration);
	}

#ifdef _DEBUG
	DebugPrintExpirationTime();
#endif
}

FBAccessTokenData::FBAccessTokenData(
	wstring const& accessToken,
	DateTime const& expiration,
	DateTime const& dataAccessExpiration)
	: _accessToken(accessToken)
	, _expirationDate(expiration)
	, _dataAccessExpirationDate(dataAccessExpiration) {
	_hasDataAccessExpirationDate = _dataAccessExpirationDate.time_since_epoch() > TimeSpan::zero();
#ifdef _DEBUG
	DebugPrintExpirationTime();
#endif
}

FBAccessTokenData::FBAccessTokenData(
	wstring const& accessToken,
	DateTime const& expiration,
	DateTime const& dataAccessExpiration,
	FBPermissions& grantedPermissions,
	FBPermissions& declinedPermissions)
	: _accessToken(accessToken.c_str())
	, _expirationDate(expiration)
	, _dataAccessExpirationDate(dataAccessExpiration)
	, _grantedPermissions(grantedPermissions)
	, _declinedPermissions(declinedPermissions) {
	_hasDataAccessExpirationDate = _dataAccessExpirationDate.time_since_epoch() > TimeSpan::zero();
#ifdef _DEBUG
	DebugPrintExpirationTime();
#endif
}

wstring FBAccessTokenData::AccessToken() const {
	return _accessToken.c_str();
}

DateTime FBAccessTokenData::ExpirationDate() const {
	return _expirationDate;
}

DateTime FBAccessTokenData::DataAccessExpirationDate() const {
	return _dataAccessExpirationDate;
}

FBPermissions FBAccessTokenData::GrantedPermissions() const {
	return _grantedPermissions;
}

FBPermissions FBAccessTokenData::DeclinedPermissions() const {
	return _declinedPermissions;
}

bool FBAccessTokenData::IsExpired() const {
	const Calendar cal;
	cal.SetToNow();
	return (cal.CompareDateTime(_expirationDate) >= 0);
}

bool FBAccessTokenData::IsDataAccessExpired() const {
	if (_hasDataAccessExpirationDate) {
		const Calendar cal;
		cal.SetToNow();
		return (cal.CompareDateTime(_dataAccessExpirationDate) >= 0);
	}
	return false;
}

void FBAccessTokenData::SetPermissions(std::vector<Graph::FBPermission> const& perms) {
	std::vector<wstring> granted;
	std::vector<wstring> declined;

	for (auto perm : perms) {
		if (compare_ordinal(perm.Status().c_str(), L"granted") == 0) {
			granted.push_back(perm.Permission());
		} else if (compare_ordinal(perm.Status().c_str(), L"declined") == 0) {
			declined.push_back(perm.Permission());
		}
	}

	_grantedPermissions = FBPermissions(granted);
	_declinedPermissions = FBPermissions(declined);
}

std::any FBAccessTokenData::FromUri(Uri const& response) {
	auto gotToken = false;
	auto gotExpiration = false;
	auto gotDataAccessExpiration = false;
	hstring token;
	hstring expiration;
	hstring dataAccessExpiration;
	std::any data;

	const auto decoder = FBAccessTokenData::ParametersFromResponse(response);

	for (unsigned int i = 0; i < decoder.Size(); i++) {
		auto entry = decoder.GetAt(i);

		if (entry.Name() == L"access_token") {
			token = entry.Value();
			gotToken = true;
		} else if (entry.Name() == L"expires_in") {
			expiration = entry.Value();
			gotExpiration = true;
		} else if (entry.Name() == L"data_access_expiration_time") {
			dataAccessExpiration = entry.Value();
			gotDataAccessExpiration = true;
		}
	}

	if (gotToken && gotExpiration && gotDataAccessExpiration) {
		data = FBAccessTokenData(token.c_str(), expiration.c_str(), dataAccessExpiration.c_str());
	}

	return data;
}

void FBAccessTokenData::CalculateExpirationDateTime(
	wstring const& expiration
	) {
	const Calendar cal;
	auto const numSecs = _wtoi(expiration.c_str());
	cal.SetToNow();
	cal.AddSeconds(numSecs);
	_expirationDate = cal.GetDateTime();
}

void FBAccessTokenData::CalculateDataAccessExpirationDateTime(
	wstring const& dataAccessExpiration
	) {
	const Calendar cal;
	auto const numSecs = _wtoi(dataAccessExpiration.c_str());
	if (numSecs != 0) {
		cal.SetToNow();
		cal.AddSeconds(numSecs);
		_dataAccessExpirationDate = cal.GetDateTime();
		_hasDataAccessExpirationDate = true;
	}
}

WwwFormUrlDecoder FBAccessTokenData::ParametersFromResponse(Uri const& response) {
	// facebook sometimes returns the access token, etc., as a Uri fragment
	// but in the query string, making it not parse correctly. Here we check
	// if this is the case (look for "?#" pattern in the string and turn it
	// into a normal Uri fragment that we can fix later
	auto responseString = wstring(response.DisplayUri().c_str());
	auto const found = responseString.find(L"?#");
	auto nonConstResponse(response);
	if (found != std::string::npos) {
		auto const uriFragment(responseString.substr(found + 1)); // +1 to move past '?' char
		auto const uriDomain(responseString.substr(0, found));
		auto const joinedUri(uriDomain + uriFragment);
		nonConstResponse = Uri(joinedUri.c_str());
	}

	auto parameters(nonConstResponse.QueryParsed());
	if (!parameters.Size()) {
		// FB sometimes returns access token, etc., in the Uri fragment
		// rather than the query string.  WinRT only lets you parse a query
		// string from a full Uri, so we'll mock one up with the fragment from
		// the original response as the query string, then parse that.
		//
		// Note that the Uri::Fragment property includes the leading '#'
		// character, inconveniently, so we have to strip this character or
		// we'll just end up with a Uri with the same fragment and an empty
		// query.
		const wstring frag(nonConstResponse.Fragment().c_str());
		std::wostringstream responseStringStream;
		responseStringStream << L"file:///dev/null?" << frag.substr(1).c_str() << std::endl;

		responseString = responseStringStream.str();

		const Uri newResponse(responseString);
		parameters = newResponse.QueryParsed();
	}
	return parameters;
}

#ifdef _DEBUG
void FBAccessTokenData::DebugPrintExpirationTime(
	) const {
	const DateTimeFormatter dtfDay(YearFormat::Default, MonthFormat::Default, DayFormat::Default, DayOfWeekFormat::Default);
	const DateTimeFormatter dtfTime(HourFormat::Default, MinuteFormat::Default, SecondFormat::Default);
	wostringstream msgStringStream;
	msgStringStream << L"Token expires at " << dtfDay.Format(_expirationDate).c_str() << L", " << dtfTime.Format(_expirationDate).c_str() << std::endl;
	const hstring msgString(msgStringStream.str().c_str());
	OutputDebugString(msgString.c_str());
}
#endif
}
