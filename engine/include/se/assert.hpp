#pragma once

#include <iostream>


#ifdef SE_DIST
	#define SE_NO_ASSERT
	#define SE_NO_SLOW_ASSERT
#elifdef SE_RELEASE
	#define SE_NO_SLOW_ASSERT
#elifdef SE_DEBUG
#else
	#error Unknown build system
#endif


#ifdef SE_WINDOWS
	#define __SE_DEBUG_BREAK asm("int 3")
#else
	#define __SE_DEBUG_BREAK __asm__("int3")
#endif



#define __SE_INTERNAL_ASSERT(expr, msg) if (!!(expr)) {}\
else {\
	std::cout << "ASSERT '" #expr "' failed in file '" __FILE__ "':" << __LINE__ << " : " msg << std::endl;\
	__SE_DEBUG_BREAK;\
}



#ifdef SE_NO_ASSERT
	#define SE_ASSERT(expr, msg)
#else
	#define SE_ASSERT(expr, msg) __SE_INTERNAL_ASSERT(expr, msg)
#endif

#ifdef SE_NO_SLOW_ASSERT
	#define SE_SLOW_ASSERT(expr, msg)
#else
	#define SE_SLOW_ASSERT(expr, msg) __SE_INTERNAL_ASSERT(expr, msg)
#endif