#include "pch.h"
#include "FBSingleValue.h"

#include <winrt/Windows.Foundation.Collections.h>

TEST(FBSingleValueTests, CreateInstance)
{
	// arrange
	winrt::hstring request(L"");

	// act
	auto instance = std::make_shared<winsdkfb::FBSingleValue>(request, nullptr, nullptr);

	// assert
	EXPECT_TRUE(instance != nullptr);
}
