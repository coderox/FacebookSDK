#include "pch.h"

#include "FBResult.h"

#include <winrt/Windows.Foundation.h>
//
//TEST(FBResultTests, CreateInstance)
//{
//	// arrange
//	winsdkfb::FBResult fbResult;
//
//	// act
//
//	// assert
//	EXPECT_NE(fbResult, nullptr);
//}

TEST(FBResultTests, CreateInstanceWithNullptrShouldNotSucceed)
{
	// arrange
	winsdkfb::FBResult fbResult;

	// act

	// assert
	EXPECT_FALSE(fbResult.Succeeded());
}

TEST(FBResultTests, TestCreateInstanceWithErrorShouldHaveError)
{
	// arrange
	auto fbError = winsdkfb::FBError(0, L"type", L"message");

	// act
	auto fbResult = winsdkfb::FBResult(fbError);
	auto error = fbResult.ErrorInfo();

	// assert
	EXPECT_FALSE(fbResult.Succeeded());
	EXPECT_EQ(error.Code(), 0);
	EXPECT_EQ(error.Message(), L"message");
	//EXPECT_EQ(fbResult.ErrorInfo().Type(), L"type");
}
