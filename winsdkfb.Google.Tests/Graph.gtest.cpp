#include "pch.h"
#include <winrt/base.h>
#include <any>
#include "Graph/FBUser.h"

#define USER_REQUEST L"{\"first_name\":\"Johan\",\"last_name\":\"Lindfors\",\"name\":\"Johan Lindfors\",\"id\":\"10156062009459646\",\"picture\":{\"data\":{\"height\":50,\"is_silhouette\":false,\"url\":\"https://platform-lookaside.fbsbx.com/platform/profilepic/?asid=10156062009459646&height=50&width=50&ext=1550996804&hash=AeTlVQ4Q_fIUqP_n\",\"width\":50}}}"
#define USER_PROFILE_PICTURE_REQUEST L"{\"height\":100,\"is_silhouette\":false,\"url\":\"https://platform-lookaside.fbsbx.com/platform/profilepic/?asid=10156062009459646&height=100&width=100&ext=1550999201&hash=AeS7Fbdntkppntf6\",\"width\":100}"
#define USER_PROFILE_PICTURE_DATA_REQUEST L"{\"data\":{\"height\":100,\"is_silhouette\":false,\"url\":\"https://platform-lookaside.fbsbx.com/platform/profilepic/?asid=10156062009459646&height=100&width=100&ext=1550999201&hash=AeS7Fbdntkppntf6\",\"width\":100}}"

TEST(GraphTests, CreateFBUserFromJson)
{
	// arrange
	std::wstring json(USER_REQUEST);

	// act
	auto user = std::any_cast<winsdkfb::Graph::FBUser>(winsdkfb::Graph::FBUser::FromJson(json));

	// assert
	EXPECT_STREQ(L"Johan", user.FirstName().c_str());
	EXPECT_STREQ(L"Lindfors", user.LastName().c_str());
	EXPECT_STREQ(L"Johan Lindfors", user.Name().c_str());
	//EXPECT_TRUE(user.Picture().Succeeded());
	//EXPECT_TRUE(user.Picture().Data().Succeeded());
	EXPECT_EQ(50, user.Picture().Data().Height());
	EXPECT_EQ(50, user.Picture().Data().Width());
}

TEST(GraphTests, CreateFBProfilePictureFromJson)
{
	// arrange
	std::wstring json(USER_PROFILE_PICTURE_REQUEST);

	// act
	auto profilePicture = std::any_cast<winsdkfb::Graph::FBProfilePicture>(winsdkfb::Graph::FBProfilePicture::FromJson(json));

	// assert
	
	EXPECT_EQ(100, profilePicture.Height());
	EXPECT_EQ(100, profilePicture.Width());
	EXPECT_EQ(false, profilePicture.IsSilhouette());
	EXPECT_FALSE(profilePicture.Url().empty());
}

TEST(GraphTests, CreateFBProfilePictureDataFromJson)
{
	// arrange
	std::wstring json(USER_PROFILE_PICTURE_DATA_REQUEST);

	// act
	auto profilePicture = std::any_cast<winsdkfb::Graph::FBProfilePictureData>(winsdkfb::Graph::FBProfilePictureData::FromJson(json));

	// assert
	EXPECT_EQ(100, profilePicture.Data().Height());
	EXPECT_EQ(100, profilePicture.Data().Width());
	EXPECT_EQ(false, profilePicture.Data().IsSilhouette());
	EXPECT_FALSE(profilePicture.Data().Url().empty());
}
