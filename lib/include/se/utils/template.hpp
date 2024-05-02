#pragma once

#include "se/types.hpp"



namespace se::utils {
	template <typename ...Args>
	constexpr se::ByteCount sizeofVariadic();
} // namespace se::utils



#include "se/utils/template.inl"