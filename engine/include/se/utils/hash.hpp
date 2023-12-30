#pragma once

#include <string>

#include "../core.hpp"
#include "../types.hpp"



namespace se
{
	/**
	 * @brief Hash a string to a 32 bits integer. Use DJB2
	*/
	SE_CORE se::Uint32 hash(const std::string &message);



} // namespace se
