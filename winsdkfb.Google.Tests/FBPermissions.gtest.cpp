#include "pch.h"
#include <vector>
#include "FBPermissions.h"

TEST(FBPermissionsTests, CreateInstance)
{
	// arrange
	
	// act
	auto permissions = winsdkfb::FBPermissions();

	// assert
	EXPECT_EQ(permissions.Values().size(), 0);
}

TEST(FBPermissionsTests, CreateInstanceFromString)
{
	// arrange
	winrt::hstring requested_permissions =
		L"public_profile, user_friends,user_likes,user_location,publish_pages,manage_pages,user_posts";

	// act
	auto permissions = winsdkfb::FBPermissions::FromString(requested_permissions);

	// assert
	EXPECT_EQ(permissions.Values().size(), 7);
}

TEST(FBPermissionsTests, CreateInstanceWithVector)
{
	// arrange
	std::vector<winrt::hstring> requested_permissions = {
		L"public_profile",
		L"user_friends",
		L"user_likes",
		L"user_location",
		L"publish_pages",
		L"manage_pages",
		L"user_posts"
	};
	auto permissions = winsdkfb::FBPermissions(requested_permissions);

	// act

	// assert
	EXPECT_EQ(permissions.Values().size(), 7);
}

TEST(FBPermissionsTests, Difference)
{
	// arrange
	std::vector<winrt::hstring> requested_permissions = {
		L"public_profile",
		L"user_friends",
		L"user_likes",
		L"user_location",
		L"publish_pages",
		L"manage_pages",
		L"user_posts"
	};

	std::vector<winrt::hstring> declined_permissions = {
		L"user_friends",
		L"publish_pages",
		L"user_posts"
	};
	
	// act
	auto permissions = winsdkfb::FBPermissions::Difference(requested_permissions, declined_permissions);

	// assert
	EXPECT_EQ(requested_permissions.size(), 7);
	EXPECT_EQ(declined_permissions.size(), 3);
	EXPECT_EQ(permissions.Values().size(), 4);
}

TEST(FBPermissionsTests, ToString)
{
	// arrange
	std::vector<winrt::hstring> requested_permissions = {
		L"public_profile",
		L"user_friends",
		L"user_posts"
	};

	// act
	auto permissions = winsdkfb::FBPermissions(requested_permissions);
	auto permissionsString = permissions.ToString();

	// assert
	EXPECT_EQ(permissions.Values().size(), 3);
	EXPECT_STREQ(permissionsString.c_str(), L"public_profile,user_friends,user_posts");
}