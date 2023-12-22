#pragma once

#include <string>

#include "../core.hpp"
#include "../types.hpp"


#define SE_UUID_BASE 16


namespace se
{
	SE_CORE std::string int64ToString(se::Uint64 number, se::Uint32 base = 10);


	
} // namespace se
