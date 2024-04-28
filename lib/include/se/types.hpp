#pragma once

#include <cinttypes>



namespace se {
	using Int8   = int8_t;
	using Uint8  = uint8_t;
	using Int16  = int16_t;
	using Uint16 = uint16_t;
	using Int32  = int32_t;
	using Uint32 = uint32_t;
	using Int64  = int64_t;
	using Uint64 = uint64_t;

	using Int = se::Int32;
	using Uint = se::Uint32;

	using Float32 = float;
	using Float64 = double;
	using Float   = se::Float32;

	using Size      = se::Uint64;
	using ByteCount = se::Uint64;

	using Byte = se::Int8;
} // namespace se
