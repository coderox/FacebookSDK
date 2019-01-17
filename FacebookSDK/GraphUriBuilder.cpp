#include "pch.h"
#include "GraphUriBuilder.h"
#include "FacebookSession.h"

#include <regex>
#include <sstream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::FacebookSDK::implementation
{
	GraphUriBuilder::GraphUriBuilder(hstring const& path)
	{
		Uri testUri{nullptr};
		bool buildDomain = false;
		try
		{
			testUri = Uri(path);
		}
		catch (hresult_invalid_argument e)
		{
			buildDomain = true;
		}

		if (buildDomain)
		{
			hstring domain(L"https://graph.facebook.com/");
			testUri = Uri(domain + path);
		}
		_host = testUri.Host();
		_path = testUri.Path();
		_scheme = testUri.SchemeName();

		// remove any extra '/'s from path
		FixPathDelimiters();
		// capture api version from path or build it
		BuildApiVersionString();
		// save query params in map for later use
		DecodeQueryParams(testUri);
	}

	Uri GraphUriBuilder::MakeUri()
	{
		// this request_host stuff is a bit of a hack to send graph calls in
		// the unit tests to our own web service.
		if (_queryParams.HasKey(L"request_host"))
		{
			_host = unbox_value<hstring>(_queryParams.Lookup(L"request_host"));
		}
		hstring fullPath = hstring(_scheme.c_str()) + L"://" + _host + L"/" + _apiVersion + _path;

		if (_queryParams.Size() > 0)
		{
			hstring separator(L"?");
			for (auto const& current : _queryParams) {
				fullPath = fullPath + separator + Uri::EscapeComponent(current.Key()) + L"=" + Uri::EscapeComponent(unbox_value<hstring>(current.Value()));
				separator = L"&";
			}
		}
		return Uri(fullPath);
	}

	void GraphUriBuilder::AddQueryParam(hstring const& query, hstring const& param)
	{
		_queryParams.Insert(query, box_value(param));
	}

	void GraphUriBuilder::BuildApiVersionString() {
		std::wstring regexString = LR"__(^.?(v\d\.\d)(.*))__";
		std::wregex apiRegex{ regexString };
		std::wsmatch match;
		std::wstring searchString{ _path.c_str() };
		std::regex_match(searchString, match, apiRegex);
		if (match.size() >= 3) // 1 for matched string + 2 for each capture group
		{
			std::wstring apiString = match[1].str();
			_apiVersion = apiString.c_str();
			// need to adjust _path so that the api version doesn't get added twice
			std::wstring newPath = match[2].str();
			_path = newPath.c_str();
		}
		else
		{
			auto sess = FacebookSession::ActiveSession();
			if (sess.APIMajorVersion())
			{
				//_apiVersion = "v" + sess.APIMajorVersion().ToString() + "." + sess.APIMinorVersion.ToString();
			}
		}
	}
	
	void GraphUriBuilder::FixPathDelimiters() { 
		hstring fixedPath;
		std::wstring originalPath{ _path.c_str() };
		std::wistringstream iss{ originalPath };
		std::wstring token;
		while (std::getline(iss, token, L'/'))
		{
			if (token.size() > 0)
			{
				fixedPath = fixedPath + L"/" + hstring(token.c_str());
			}
		}
		_path = fixedPath;
	}

	void GraphUriBuilder::DecodeQueryParams(Uri const& uri) {
		WwwFormUrlDecoder decoder(uri.QueryParsed());
		if (decoder.Size() > 0)
		{
			for (auto const& current : decoder)
			{
				_queryParams.Insert(current.Name(), box_value(current.Value()));
			}
		}
	}
}
