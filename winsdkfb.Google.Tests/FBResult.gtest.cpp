#include "pch.h"

#include "FBResult.h"

#include <winrt/Windows.Foundation.h>

TEST(FBResultTests, CreateInstance)
{
	// arrange
	auto fbResult = std::make_shared<winsdkfb::FBResult>(nullptr);

	// act

	// assert
	EXPECT_NE(fbResult, nullptr);
}

TEST(FBResultTests, CreateInstanceWithNullptrShouldNotSucceed)
{
	// arrange
	auto fbResult = std::make_shared<winsdkfb::FBResult>(nullptr);

	// act

	// assert
	EXPECT_NE(fbResult, nullptr);
	EXPECT_FALSE(fbResult->Succeeded());
}

TEST(FBResultTests, TestCreateInstanceWithErrorShouldHaveError)
{
	// arrange
	auto fbError = std::make_shared<winsdkfb::FBError>(0, L"type", L"message");

	// act
	auto fbResult = std::make_shared<winsdkfb::FBResult>(fbError);

	// assert
	EXPECT_NE(fbResult, nullptr);
	EXPECT_FALSE(fbResult->Succeeded());
	EXPECT_NE(fbResult->ErrorInfo(), nullptr);
	EXPECT_EQ(fbResult->ErrorInfo()->Type(), L"type");
}
