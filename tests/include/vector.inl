#include "test.hpp"


START_TEST("vector")
	se::Vec4f vec4 {0.f};
	TEST(vec4 == vec4)
END_TEST("vector")