#pragma once

#ifndef SE_APPLE

#include "se/types.hpp"

#define SE_REQUIRES(val) requires (val)


namespace se::concepts {
	template <typename T>
	concept Arithmetic = requires(T lhs, T rhs) {
		lhs = rhs;
		lhs += rhs;
		lhs -= rhs;
	};

	template <typename T>
	concept Number = requires {
		requires std::same_as<T, se::Int8>
			|| std::same_as<T, se::Uint8>
			|| std::same_as<T, se::Int16>
			|| std::same_as<T, se::Uint16>
			|| std::same_as<T, se::Int32>
			|| std::same_as<T, se::Uint32>
			|| std::same_as<T, se::Int64>
			|| std::same_as<T, se::Uint64>
			|| std::same_as<T, se::Float32>
			|| std::same_as<T, se::Float64>;
	};

	template <typename T>
	concept Allocator = requires(T obj) {
		T::Handle;
		{obj.allocate(static_cast<se::ByteCount> (0))} -> std::convertible_to<typename T::Handle>;
		{obj.free(typename T::Handle())} -> std::same_as<void>;
		{obj.reallocate(typename T::Handle(), static_cast<se::ByteCount> (0))} -> std::same_as<void>;
	};


	template <typename T>
	concept Ratio = requires(T obj) {
		{T::num}    ;//-> std::same_as<se::Uint>;
		{T::den}    ;//-> std::same_as<se::Uint>;
		{T::ratio}  ;//-> std::same_as<se::Float>;
		{T::prefix} ;//-> std::same_as<std::string>;
	};
} // namespace se::concepts


#else
	#define SE_REQUIRES(val)
#endif