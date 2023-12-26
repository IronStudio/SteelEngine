#pragma once

#include <string>

#include "../core.hpp"
#include "../types.hpp"


#define SE_UUID_BASE 16


namespace se
{
	/**
	 * @brief Convert an integer to a string with an arbitrary bases
	 * @warning This should be used only for non-base-10 convertion. If you want to convert to base 10, prefer
	 *          using `std::to_string`
	*/
	SE_CORE std::string int64ToString(se::Uint64 number, se::Uint32 base = 10);


	
} // namespace se
