#include "se/utils/files.hpp"



namespace se::utils {
	std::vector<se::Byte> readBinaryFile(std::ifstream &file) {
		std::vector<se::Byte> buffer {std::istreambuf_iterator<char> (file), {}};
		return buffer;
	}

} // namespace se::utils
