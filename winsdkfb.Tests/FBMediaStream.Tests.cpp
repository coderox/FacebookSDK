#include "pch.h"
#include "CppUnitTest.h"
#include "../winsdkfb/FBMediaStream.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Storage::Streams;
namespace winsdkfb_Tests
{
	TEST_CLASS(FBMediaStream_Tests)
	{
	public:
		TEST_METHOD(TestCreateInstance)
		{
			// arrange
			auto fileName = ref new Platform::String(L"Test.mpeg");
			auto memoryStream = ref new InMemoryRandomAccessStream();
			auto streamReference = RandomAccessStreamReference::CreateFromStream(memoryStream);
			auto stream = concurrency::create_task([streamReference]() { return streamReference->OpenReadAsync(); }).get();

			// act
			auto instance = ref new winsdkfb::FBMediaStream(fileName, stream);

			// assert
			Assert::IsNotNull(instance);
			Assert::IsNotNull(instance->Stream);
			Assert::AreEqual(L"Test.mpeg", instance->FileName);
		}
	};
}
