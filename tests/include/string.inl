#include <se/essentials/string.hpp>

#include "test.hpp"



SE_TEST_BEGIN(stringTest)

	SE_TEST(se::UTF8String str {}, str.isEmpty())
	SE_TEST(se::UTF8String str1 {"Hello world !"}, str1.getLength() == 13)
	SE_TEST_CONDITION(str1.getSizeInBytes() == 14)
	SE_TEST(se::UTF8String str2 {"Bonjour à tous !"}, str2.getLength() == 16)
	SE_TEST_CONDITION(str2.getSizeInBytes() == 18)

SE_TEST_END(stringTest)