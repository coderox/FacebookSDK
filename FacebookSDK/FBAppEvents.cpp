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
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Globalization.DateTimeFormatting.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::ApplicationModel::Store;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Data::Json;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Services::Store;
using namespace Windows::Storage;
using namespace Windows::System::UserProfile;
using namespace Windows::System::Threading;
using namespace Windows::Globalization::DateTimeFormatting;

#ifdef _DEBUG
#define ALWAYS_LOG_INSTALLS
#endif

// Constants
#define FACEBOOK_ACTIVITIES_PATH L"/activities"
#define FACEBOOK_MOBILE_APP_INSTALL L"MOBILE_APP_INSTALL"
#define FACEBOOK_CUSTOM_APP_EVENTS L"CUSTOM_APP_EVENTS"

#pragma region GetCampaignIdHelpers

hstring GetCampaignId(bool useSimulator)
{
	hstring campaignIdField(L"customPolicyField1");
	if (ApiInformation::IsTypePresent(L"Windows.Services.Store.StoreContext"))
	{
		StoreContext ctx = StoreContext::GetDefault();
		StoreProductResult productResult = ctx.GetStoreProductForCurrentAppAsync().get();
		if (productResult != nullptr && productResult.Product() != nullptr)
		{
			for (auto const& sku : productResult.Product().Skus())
			{
				if (sku.IsInUserCollection()) {
					return sku.CollectionData().CampaignId();
				}
			}
			// Yes, it is OK to return "" without checking the license when the user collection is present.
			// the AppLicense fallback below won't apply.
			return L"";
		}

		StoreAppLicense appLicense = ctx.GetAppLicenseAsync().get();

		// This backup method is used for purchases that did not have an MSA; there was no user.
		if (appLicense != nullptr && !appLicense.ExtendedJsonData().empty())
		{
			JsonObject json{ nullptr };
			if (JsonObject::TryParse(appLicense.ExtendedJsonData(), json))
			{
				if (json.HasKey(campaignIdField))
				{
					return json.GetNamedString(campaignIdField);
				}
			}
		}
		return L"";
	}
	else
	{
		if (useSimulator)
		{
			return CurrentAppSimulator::GetAppPurchaseCampaignIdAsync().get();
		}
		else
		{
			return CurrentApp::GetAppPurchaseCampaignIdAsync().get();
		}
	}
}

IAsyncOperation<hstring> GetCampaignIdAsync(bool useSimulator)
{
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
		auto session = FBSession::ActiveSession();
		auto appId = session.FBAppId();

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

	IAsyncAction FBAppEvents::PublishInstall(hstring appId)
	{
		hstring lastPingKey(L"LastAttributionPing" + appId);
		hstring lastResponseKey(L"LastInstallResponse" + appId);
		ApplicationDataContainer settings = FBSession::DataContainer();

		hstring pingTime;
		if (settings.Values().HasKey(lastPingKey)) {
			pingTime = unbox_value<hstring>(settings.Values().Lookup(lastPingKey));
		}
#ifndef ALWAYS_LOG_INSTALLS
		if (pingTime.empty())
#endif
		{
			auto lastAttributionResponse = co_await FBAppEvents::LogInstallEvent(appId);
			Calendar calendar;
			calendar.SetToNow();
			auto universalTime = calendar.GetDateTime().time_since_epoch().count();
			settings.Values().Insert(lastPingKey, PropertyValue::CreateString(to_hstring(universalTime)));
			settings.Values().Insert(lastResponseKey, PropertyValue::CreateString(lastAttributionResponse));

#ifdef _DEBUG
			auto formatter = DateTimeFormatter(L"year month day hour minute second");
			hstring msg(L"Mobile App Install Response: " + lastAttributionResponse + L"\n"
				L"Mobile App Install Ping Time: " + formatter.Format(calendar.GetDateTime()) + L"\n");
			//OutputDebugString(msg.c_str());
#endif
		}
	}

	IAsyncAction FBAppEvents::LogActivateEvent(hstring appId)
	{
		hstring path(appId + FACEBOOK_ACTIVITIES_PATH);
		PropertySet parameters;
		parameters.Insert(L"event", box_value(FACEBOOK_CUSTOM_APP_EVENTS));
		parameters.Insert(L"custom_events", box_value(FBAppEvents::GetActivateAppJson()));
		parameters.Insert(L"advertiser_id", box_value(AdvertisingManager::AdvertisingId()));
		parameters.Insert(
			L"advertiser_tracking_enabled",
			box_value(AdvertisingManager::AdvertisingId().empty() ? L"0" : L"1")
		);

		auto response = co_await HttpManager::Instance().PostTaskAsync(path, parameters.GetView());
#ifdef _DEBUG
		hstring msg(L"Custom App Event Response: " + response);
		//OutputDebugString(msg.c_str());
#endif
	}

	IAsyncOperation<hstring> FBAppEvents::LogInstallEvent(hstring appId)
	{
		hstring path(appId + FACEBOOK_ACTIVITIES_PATH);
		PropertySet parameters;
		parameters.Insert(L"event", box_value(FACEBOOK_MOBILE_APP_INSTALL));
		parameters.Insert(L"advertiser_id", box_value(AdvertisingManager::AdvertisingId()));
		parameters.Insert(
			L"advertiser_tracking_enabled",
			box_value(AdvertisingManager::AdvertisingId().empty() ? L"0" : L"1")
		);

		try {
			hstring campaignId = co_await GetCampaignIdAsync(FBAppEvents::UseSimulator());
			parameters.Insert(L"windows_attribution_id", box_value(campaignId));
			hstring postResult = co_await HttpManager::Instance().PostTaskAsync(path, parameters.GetView());
			co_return postResult;
		}
		catch (hresult_error ex) {
			//OutputDebugString(L"This can happen when the app is not yet published. If that is the case, ignore it.");
			//OutputDebugString(ex.message().c_str());
		}
		//OutputDebugString(L"This value must be replaced");
		co_return L"";
	}

	hstring FBAppEvents::GetActivateAppJson()
	{
		JsonArray customEvents;
		JsonObject activateJson;
		activateJson.SetNamedValue(
			L"_eventName",
			JsonValue::CreateStringValue(L"fb_mobile_activate_app")
		);
		customEvents.Append(activateJson);
		return customEvents.ToString();
	}
}
