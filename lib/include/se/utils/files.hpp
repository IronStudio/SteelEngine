#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "se/types.hpp"



namespace se::utils {
	std::vector<se::Byte> readBinaryFile(std::ifstream &file);
} // namespace se::utils
