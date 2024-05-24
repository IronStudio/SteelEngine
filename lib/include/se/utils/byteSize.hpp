#pragma once

#include <ostream>

#include "se/types.hpp"
#include "se/units.hpp"



namespace se::literals {
	inline se::ByteCount operator ""_kiB(long double val);
	inline se::ByteCount operator "" _MiB(long double val);
	inline se::ByteCount  operator ""_GiB(long double val);
	inline se::ByteCount operator ""_kB(long double val);
	inline se::ByteCount operator "" _MB(long double val);
	inline se::ByteCount  operator ""_GB(long double val);

	inline se::ByteCount operator ""_B(unsigned long long int val);
	inline se::ByteCount operator ""_kiB(unsigned long long int val);
	inline se::ByteCount operator "" _MiB(unsigned long long int val);
	inline se::ByteCount  operator ""_GiB(unsigned long long int val);
	inline se::ByteCount operator ""_kB(unsigned long long int val);
	inline se::ByteCount operator "" _MB(unsigned long long int val);
	inline se::ByteCount  operator ""_GB(unsigned long long int val);
} // namespace se::literals


#include "se/utils/byteSize.inl"