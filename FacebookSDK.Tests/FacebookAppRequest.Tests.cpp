#include "pch.h"
#include "CppUnitTest.h"
#include "../FacebookSDK/FacebookAppRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FacebookSDK_Tests
{
	TEST_CLASS(FacebookAppRequest_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto uri = ref new Windows::Foundation::Uri("https://candycrush4.king.com/fb/connect/login_success.html?request=1000&to[0]=1001&to[1]=1002#");
			
			// act
			auto request = FacebookSDK::FacebookAppRequest::FromRequestDialogResponse(uri);
			auto requestId = request->RequestId;
			auto recipients = request->RecipientIds;

			// assert
			Assert::IsNotNull(request);
			Assert::AreEqual("1000", requestId);
			Assert::AreEqual(2, (int)recipients->Size);
			Assert::AreEqual("1001", recipients->GetAt(0));
			Assert::AreEqual("1002", recipients->GetAt(1));
		}
	};
}