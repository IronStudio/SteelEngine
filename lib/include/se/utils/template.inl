#include "se/utils/template.hpp"



namespace se::utils {
	template <typename ...Args>
	constexpr se::ByteCount sizeofVariadic() {
		return (sizeof(Args) + ...);
	}

} // namespace se::utils
