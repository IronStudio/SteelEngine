#pragma once

#include <iostream>
#include <string>



#ifndef NDEBUG
	#define SE_ASSERT(cond, msg) if (!(cond)) {\
		std::cerr << "Assertion failed : " << #cond << " > " << msg << std::endl;\
		__asm("int3");\
	}
#else
	#define SE_ASSERT(cond, msg)
#endif


