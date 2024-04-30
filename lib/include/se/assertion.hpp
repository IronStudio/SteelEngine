#pragma once

#include <iostream>
#include <string>



#define SE_ASSERT(cond, msg) if (!(cond)) {\
	std::cerr << "Assertion failed : " << #cond << " > " << msg << std::endl;\
	__asm("int3");\
}


