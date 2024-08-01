#pragma once


#ifdef __APPLE__
	#define SE_REQUIRE(...)
#else
	#define SE_REQUIRE(...) requires (__VA_ARGS__)
#endif