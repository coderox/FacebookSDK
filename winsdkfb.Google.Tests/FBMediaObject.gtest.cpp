#include "pch.h"
#include "FBMediaObject.h"

TEST(FBMediaObjectTests, CreateInstance)
{
	// arrange
	auto instance = std::make_shared<winsdkfb::FBMediaObject>();

	// act

	// assert
	EXPECT_TRUE(instance != nullptr);
}

TEST(FBMediaObjectTests, SetValueWithBytes)
{
	// arrange
	auto instance = std::make_shared<winsdkfb::FBMediaObject>();
	std::vector<unsigned char> vector = { 0,1,2,3 };

	// act
	instance->SetValue(vector);
	auto resultValue = instance->GetValue();

	// assert
	EXPECT_TRUE(instance != nullptr);
	for (unsigned int i = 0; i < vector.size(); i++) {
		EXPECT_EQ(vector[i], resultValue[i]);
	}
}
