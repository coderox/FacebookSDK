#pragma once

#include "FacebookAppEvents.g.h"

namespace winrt::FacebookSDK::implementation
{
	struct FacebookAppEvents : FacebookAppEventsT<FacebookAppEvents>
	{
		FacebookAppEvents() = default;

		static bool UseSimulator();
		static void UseSimulator(bool value);

		static void ActiveApp();

	private:
		/**
		 * Publish an install event to the Facebook graph endpoint.
		 * Write the timestamp to localSettings so we only trigger this once.
		 * @param The Application Id for the request
		 * @return The IAsyncAction that can be waited on for the completion
		 */
		static winrt::Windows::Foundation::IAsyncAction PublishInstall(
			winrt::hstring AppId
		);

		/**
		 * Logs a custom app event to the Facebook graph endpoint.
		 * @param The Application Id for the request
		 * @return The IAsyncAction that can be waited on for the completion
		 */
		static winrt::Windows::Foundation::IAsyncAction LogActivateEvent(
			winrt::hstring AppId
		);

		/**
		 * Logs an install event to the Facebook graph endpoint.
		 * The user will be looked up using idfa or windows_attribution_id
		 * @param The Application Id for the request
		 * @return The HTTP response from the request
		 */
		static winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> LogInstallEvent(
			winrt::hstring AppId
		);

		/**
		 * Creates a JSON array encapsulating the activate app event
		 * @return The JSON string for the event
		 */
		static winrt::hstring GetActivateAppJson();

		static bool _useSimulator;
	};
}

namespace winrt::FacebookSDK::factory_implementation
{
	struct FacebookAppEvents : FacebookAppEventsT<FacebookAppEvents, implementation::FacebookAppEvents>
	{
	};
}
