#include "se/utils/version.hpp"

#include <sstream>

#include "se/exceptions.hpp"



namespace se::utils {
	bool Version::operator==(const se::utils::Version &version) const noexcept {
		return major == version.major && minor == version.minor && patch == version.patch;
	}


	auto Version::operator<=>(const se::utils::Version &version) const noexcept {
		se::Uint32 thisAsNumber {(patch & 0x0000ffff) | ((minor << 16) & 0x00ff0000) | ((major << 24) & 0xff000000)};
		se::Uint32 versionAsNumber {(version.patch & 0x0000ffff) | ((version.minor << 16) & 0x00ff0000) | ((version.major << 24) & 0xff000000)};
		return thisAsNumber <=> versionAsNumber;
	}


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
