#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define USER_REQUEST L"{\"first_name\":\"Johan\",\"last_name\":\"Lindfors\",\"name\":\"Johan Lindfors\",\"id\":\"10156062009459646\",\"picture\":{\"data\":{\"height\":50,\"is_silhouette\":false,\"url\":\"https:\/\/platform-lookaside.fbsbx.com\/platform\/profilepic\/?asid=10156062009459646&height=50&width=50&ext=1550996804&hash=AeTlVQ4Q_fIUqP_n\",\"width\":50}}}"
#define USER_PROFILE_PICTURE_REQUEST L"{\"height\":100,\"is_silhouette\":false,\"url\":\"https:\/\/platform-lookaside.fbsbx.com\/platform\/profilepic\/?asid=10156062009459646&height=100&width=100&ext=1550999201&hash=AeS7Fbdntkppntf6\",\"width\":100}"
#define USER_PROFILE_PICTURE_DATA_REQUEST L"{\"data\":{\"height\":100,\"is_silhouette\":false,\"url\":\"https:\/\/platform-lookaside.fbsbx.com\/platform\/profilepic\/?asid=10156062009459646&height=100&width=100&ext=1550999201&hash=AeS7Fbdntkppntf6\",\"width\":100}}"

namespace FacebookSDK_Tests
{
	TEST_CLASS(Graph_Tests)
	{
	public:
		TEST_METHOD(TestCreateFBUserFromJson)
		{
			// arrange
			auto json = ref new Platform::String(USER_REQUEST);

			// act
			auto user = dynamic_cast<FacebookSDK::Graph::FBUser^>(FacebookSDK::Graph::FBUser::FromJson(json));

			// assert
			Assert::IsNotNull(user);
			Assert::AreEqual("Johan", user->FirstName);
			Assert::AreEqual("Lindfors", user->LastName);
			Assert::AreEqual("Johan Lindfors", user->Name);
			Assert::IsNotNull(user->Picture);
			Assert::IsNotNull(user->Picture->Data);
			Assert::AreEqual(50, user->Picture->Data->Height);
			Assert::AreEqual(50, user->Picture->Data->Width);
		}

		TEST_METHOD(TestCreateFBProfilePictureFromJson)
		{
			// arrange
			auto json = ref new Platform::String(USER_PROFILE_PICTURE_REQUEST);

			// act
			auto profilePicture = dynamic_cast<FacebookSDK::Graph::FBProfilePicture^>(FacebookSDK::Graph::FBProfilePicture::FromJson(json));

			// assert
			Assert::IsNotNull(profilePicture);
			Assert::AreEqual(100, profilePicture->Height);
			Assert::AreEqual(100, profilePicture->Width);
			Assert::AreEqual(false, profilePicture->IsSilhouette);
			Assert::IsFalse(profilePicture->Url->IsEmpty());
		}

		TEST_METHOD(TestCreateFBProfilePictureDataFromJson)
		{
			// arrange
			auto json = ref new Platform::String(USER_PROFILE_PICTURE_DATA_REQUEST);

			// act
			auto profilePicture = dynamic_cast<FacebookSDK::Graph::FBProfilePictureData^>(FacebookSDK::Graph::FBProfilePictureData::FromJson(json));

			// assert
			Assert::IsNotNull(profilePicture);
			Assert::IsNotNull(profilePicture->Data);
			Assert::AreEqual(100, profilePicture->Data->Height);
			Assert::AreEqual(100, profilePicture->Data->Width);
			Assert::AreEqual(false, profilePicture->Data->IsSilhouette);
			Assert::IsFalse(profilePicture->Data->Url->IsEmpty());
		}
	};
}
