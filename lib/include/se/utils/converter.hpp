#pragma once

#include <vector>

#include "se/core.hpp"
#include "se/types.hpp"



namespace se::utils {
	template <typename T>
	SE_CORE std::vector<se::Byte> vectorize(const T &value);

} // namespace se::utils


#include "se/utils/converter.inl"