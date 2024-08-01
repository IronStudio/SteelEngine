#include "se/utils/endianness.hpp"

#include "se/types.hpp"



namespace se {
	constexpr se::Endianness getCompileTimeEndianness() {
		se::Uint16 value {0xFF00};
		if (*reinterpret_cast<se::Uint8*> (&value) == 0xFF)
			return se::Endianness::eBig;
		return se::Endianness::eLittle;
	}

	se::Endianness getRuntimeEndianness() {
		static se::Endianness endianness {};
		static bool firstRun {true};
		if (!firstRun)
			return endianness;

		firstRun = false;
		se::Uint16 value {0xFF00};
		if (*reinterpret_cast<se::Uint8*> (&value) == 0xFF)
			endianness = se::Endianness::eBig;
		else
			endianness = se::Endianness::eLittle;
		return endianness;
	}

} // namespace se
