#include "se/utils/version.hpp"

#include <sstream>

#include "se/exceptions.hpp"



namespace se::utils {
	std::ostream &operator<<(std::ostream &stream, const se::utils::Version &version) {
		stream << version.major << "." << version.minor << "." << version.patch;
		return stream;
	}

} // namespace se::utils


namespace se::literals {
	constexpr se::utils::Version operator ""_v(const char *text, size_t length) {
		std::string literal {text, length};
		se::utils::Version version {};

		std::istringstream stream {literal};
		std::string line {};

		int i {0};

		while (std::getline(stream, line, '.')) {
			if (i >= 3)
				throw se::exceptions::RuntimeError("Invalid version literal");
			reinterpret_cast<se::Int*> (&version)[i] = std::stoi(line);
			++i;
		}

		return version;
	}
};
