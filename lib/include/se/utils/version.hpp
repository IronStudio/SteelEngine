#pragma once

#include <ostream>

#include "se/types.hpp"



namespace se::utils {
	struct Version {
		se::Int major;
		se::Int minor;
		se::Int patch;
	};

	inline std::ostream &operator<<(std::ostream &stream, const se::utils::Version &version);

} // namespace se::utils


namespace se::literals {
	constexpr se::utils::Version operator ""_v(const char *text, size_t length);
};


#include "se/utils/version.inl"