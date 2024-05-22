#include <gtest/gtest.h>

#include <se/utils/version.hpp>

using namespace se::literals;



TEST(Utils, Version) {
	se::utils::Version v1 {}, v2 {};
	ASSERT_EQ(se::utils::Version(1, 2, 3), se::utils::Version(1, 2, 3)) << "Equality operator isn't working";
	ASSERT_EQ(se::utils::Version(81, 40, 1382), se::utils::Version(81, 40, 1382)) << "Equality operator isn't working";

	EXPECT_TRUE(se::utils::Version(81, 40, 1382) > se::utils::Version(1, 2, 3)) << " > operator not working";
	EXPECT_FALSE(se::utils::Version(81, 40, 1382) <= se::utils::Version(1, 2, 3)) << " <= operator not working";
	EXPECT_TRUE(se::utils::Version(1, 2, 3) < se::utils::Version(81, 40, 1382)) << " < operator not working";
	EXPECT_FALSE(se::utils::Version(1, 2, 3) >= se::utils::Version(81, 40, 1382)) << " >= operator not working";

	EXPECT_EQ("1.2.3"_v, se::utils::Version(1, 2, 3)) << "String literals not working";
}