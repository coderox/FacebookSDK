#include "FBAppEvents.h"
#include "FBSession.h"
#include "HttpManager.h"
#include "SDKMessage.h"

#include <winrt/Windows.ApplicationModel.Resources.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.ApplicationModel.Store.h>
#include <winrt/Windows.Services.Store.h>
#include <winrt/Windows.System.UserProfile.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#undef GetObject
#include <winrt/Windows.Data.Json.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Metadata;
using namespace Collections;
using namespace concurrency;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::ApplicationModel::Store;
using namespace Windows::Data::Json;
using namespace Windows::Globalization;
using namespace DateTimeFormatting;
using namespace Windows::Services::Store;
using namespace Windows::Storage;
using namespace Windows::System::UserProfile;
using namespace Windows::System::Threading;

#ifdef _DEBUG
#define ALWAYS_LOG_INSTALLS
#endif

// Constants
constexpr auto FACEBOOK_ACTIVITIES_PATH = L"/activities";
constexpr auto FACEBOOK_MOBILE_APP_INSTALL = L"MOBILE_APP_INSTALL";
constexpr auto FACEBOOK_CUSTOM_APP_EVENTS = L"CUSTOM_APP_EVENTS";

#pragma region GetCampaignIdHelpers

wstring GetCampaignId(bool useSimulator)
{
	const hstring campaignIdField(L"customPolicyField1");
	if (ApiInformation::IsTypePresent(L"Windows.Services.Store.StoreContext"))
	{
		const auto ctx = StoreContext::GetDefault();
		const auto productResult = ctx.GetStoreProductForCurrentAppAsync().get();
		if (productResult != nullptr && productResult.Product() != nullptr)
		{
			for (auto const& sku : productResult.Product().Skus())
			{
				if (sku.IsInUserCollection()) {
					return sku.CollectionData().CampaignId().c_str();
				}
			}
			// Yes, it is OK to return "" without checking the license when the user collection is present.
			// the AppLicense fallback below won't apply.
			return L"";
		}

		const auto appLicense = ctx.GetAppLicenseAsync().get();

		// This backup method is used for purchases that did not have an MSA; there was no user.
		if (appLicense != nullptr && !appLicense.ExtendedJsonData().empty())
		{
			JsonObject json{ nullptr };
			if (JsonObject::TryParse(appLicense.ExtendedJsonData(), json))
			{
				if (json.HasKey(campaignIdField))
				{
					return json.GetNamedString(campaignIdField).c_str();
				}
			}
		}
		return L"";
	}
	else
	{
		if (useSimulator)
		{
			return CurrentAppSimulator::GetAppPurchaseCampaignIdAsync().get().c_str();
		}
		else
		{
			return CurrentApp::GetAppPurchaseCampaignIdAsync().get().c_str();
		}
	}
}

task<wstring> GetCampaignIdAsync(bool useSimulator) {
	co_return GetCampaignId(useSimulator);
}

#pragma endregion GetCampaignIdHelpers


namespace winsdkfb
{
	bool FBAppEvents::_useSimulator = false;

	bool FBAppEvents::UseSimulator()
	{
		return _useSimulator;
	}

	void FBAppEvents::UseSimulator(bool value)
	{
		_useSimulator = value;
	}

	void FBAppEvents::ActivateApp()
	{
		// Try to grab the application id from session.
		const auto session = FBSession::ActiveSession();
		auto appId = session->FBAppId();

		if (appId.empty())
		{
			throw hresult_invalid_argument(SDKMessageMissingAppID);
		}

		//Install tracking should not fail or throw so here we catch
		//and silently ignore most errors..
		try
		{
			ThreadPool::RunAsync([appId](IAsyncAction const&) {
				FBAppEvents::PublishInstall(appId);
				FBAppEvents::LogActivateEvent(appId);
			});
		}
		catch (hresult_error ex)
		{
#if _DEBUG
			throw;
#endif
		}
		catch (...)
		{
		}
	}

	IAsyncAction FBAppEvents::PublishInstall(wstring appId)
	{
		const auto lastPingKey(L"LastAttributionPing" + appId);
		const auto lastResponseKey(L"LastInstallResponse" + appId);
		const auto settings = FBSession::DataContainer();

#ifndef ALWAYS_LOG_INSTALLS
		if (pingTime.empty())
#endif
		{
			const auto lastAttributionResponse = co_await FBAppEvents::LogInstallEvent(appId);
			const Calendar calendar;
			calendar.SetToNow();
			const auto universalTime = calendar.GetDateTime().time_since_epoch().count();
			settings.Values().Insert(lastPingKey, PropertyValue::CreateString(to_hstring(universalTime)));
			settings.Values().Insert(lastResponseKey, PropertyValue::CreateString(lastAttributionResponse));

#ifdef _DEBUG
			const auto formatter = DateTimeFormatter(L"year month day hour minute second");
			const auto msg(L"Mobile App Install Response: " + lastAttributionResponse + L"\n"
				L"Mobile App Install Ping Time: " + formatter.Format(calendar.GetDateTime()) + L"\n");
			OutputDebugString(msg.c_str());
#endif
		}
	}

	IAsyncAction FBAppEvents::LogActivateEvent(wstring appId)
	{
		const auto path(appId + FACEBOOK_ACTIVITIES_PATH);
		unordered_map<wstring, wstring> parameters;
		parameters[L"event"] = FACEBOOK_CUSTOM_APP_EVENTS;
		parameters[L"custom_events"] = FBAppEvents::GetActivateAppJson();
		parameters[L"advertiser_id"] = AdvertisingManager::AdvertisingId();
		parameters[L"advertiser_tracking_enabled"] = 
			AdvertisingManager::AdvertisingId().empty() ? L"0" : L"1";

		const auto response = co_await HttpManager::Instance()->PostTaskAsync(path, parameters);
#ifdef _DEBUG
		const auto msg(L"Custom App Event Response: " + response);
		OutputDebugString(msg.c_str());
#endif
	}

	task<wstring> FBAppEvents::LogInstallEvent(wstring appId)
	{
		const auto path(appId + FACEBOOK_ACTIVITIES_PATH);
		unordered_map<wstring, wstring> parameters;
		parameters[L"event"] = FACEBOOK_MOBILE_APP_INSTALL;
		parameters[L"advertiser_id"] = AdvertisingManager::AdvertisingId();
		parameters[L"advertiser_tracking_enabled"] = 
			AdvertisingManager::AdvertisingId().empty() ? L"0" : L"1";

		try {
			const auto campaignId = co_await GetCampaignIdAsync(FBAppEvents::UseSimulator());
			parameters[L"windows_attribution_id"] = campaignId;
			auto postResult = co_await HttpManager::Instance()->PostTaskAsync(path, parameters);
			co_return postResult;
		}
		catch (hresult_error ex) {
			OutputDebugString(L"This can happen when the app is not yet published. If that is the case, ignore it.");
			OutputDebugString(ex.message().c_str());
		}
		OutputDebugString(L"This value must be replaced");
		co_return L"";
	}

	wstring FBAppEvents::GetActivateAppJson()
	{
		const JsonArray customEvents;
		const JsonObject activateJson;
		activateJson.SetNamedValue(
			L"_eventName",
			JsonValue::CreateStringValue(L"fb_mobile_activate_app")
		);
		customEvents.Append(activateJson);
		return customEvents.ToString().c_str();
	}
}
