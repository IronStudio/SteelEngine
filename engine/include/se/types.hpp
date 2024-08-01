#pragma once

#include <cinttypes>
#include <stdfloat>



namespace se {
	using Uint8  = std::uint8_t;
	using Int8   = std::int8_t;
	using Uint16 = std::uint16_t;
	using Int16  = std::int16_t;
	using Uint32 = std::uint32_t;
	using Int32  = std::int32_t;
	using Uint64 = std::uint64_t;
	using Int64  = std::int64_t;

	using Fuint8  = std::uint_fast8_t;
	using Fint8   = std::int_fast8_t;
	using Fuint16 = std::uint_fast16_t;
	using Fint16  = std::int_fast16_t;
	using Fuint32 = std::uint_fast32_t;
	using Fint32  = std::int_fast32_t;
	using Fuint64 = std::uint_fast64_t;
	using Fint64  = std::int_fast64_t;

	using Float16  = std::float16_t;
	using Float32  = std::float32_t;
	using Float64  = std::float64_t;
	using Float128 = std::float128_t;

	using Uint  = se::Uint32;
	using Int   = se::Int32;
	using Fuint = se::Fuint32;
	using Fint  = se::Int32;
	using Float = se::Float32;

	using Count     = se::Uint64;
	using ByteCount = se::Uint64;

	using Byte = se::Uint8;

	using Char   = char;
	using Char8  = char;
	using Char16 = char16_t;
	using Char32 = char32_t;

} // namespace se
