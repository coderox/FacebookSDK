#pragma once

#include <winrt/Windows.Foundation.h>
#include <pplawait.h>

namespace winsdkfb
{
	struct FBAppEvents
	{
		FBAppEvents() = default;

		static bool UseSimulator();
		static void UseSimulator(bool value);

		static void ActivateApp();

	private:
		/**
		 * Publish an install event to the FB graph endpoint.
		 * Write the timestamp to localSettings so we only trigger this once.
		 * @param appId The Application Id for the request
		 * @return The IAsyncAction that can be waited on for the completion
		 */
		static winrt::Windows::Foundation::IAsyncAction PublishInstall(
			std::wstring appId
		);

		/**
		 * Logs a custom app event to the FB graph endpoint.
		 * @param appId The Application Id for the request
		 * @return The IAsyncAction that can be waited on for the completion
		 */
		static winrt::Windows::Foundation::IAsyncAction LogActivateEvent(
			std::wstring appId
		);

		/**
		 * Logs an install event to the FB graph endpoint.
		 * The user will be looked up using IDFA or windows_attribution_id
		 * @param appId The Application Id for the request
		 * @return The HTTP response from the request
		 */
		static concurrency::task<std::wstring> LogInstallEvent(
			std::wstring appId
		);

		/**
		 * Creates a JSON array encapsulating the activate app event
		 * @return The JSON string for the event
		 */
		static std::wstring GetActivateAppJson();

		static bool _useSimulator;
	};
}
