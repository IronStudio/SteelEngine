#pragma once



namespace se {
	enum class Endianness {
		eLittle,
		eBig
	};

	constexpr se::Endianness getCompileTimeEndianness();
	se::Endianness getRuntimeEndianness();

} // namespace se



#include "se/utils/endianness.inl"