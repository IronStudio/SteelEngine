#include "se/utils/converter.hpp"



namespace se::utils {
	template <typename T>
	std::vector<se::Byte> vectorize(const T &value) {
		std::vector<se::Byte> output {};
		output.assign((se::Byte*)&value, (se::Byte*)(&value + 1));
		return output;
	}

} // namespace se::utils
