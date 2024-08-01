#pragma once

#include "se/types.hpp"


#ifndef __APPLE__

#include <concepts>


namespace se::iterator {
	namespace {
		template <typename T>
		concept IsPointer = std::is_pointer_v<T>;
	}

	template <typename T>
	concept ForwardIterator = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && requires(T it, const T cstIt) {
		{cstIt.next()}               -> std::same_as<T>;
		{++it}                       -> std::same_as<const T&>;
		{it++}                       -> std::same_as<T>;
		{cstIt == cstIt}             -> std::same_as<bool>;
		{cstIt.getLinkedContainer()} -> se::iterator::IsPointer;
		*cstIt;
		cstIt.operator->();
	};

	template <typename T>
	concept BackwardIterator = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && requires(T it, const T cstIt) {
		{cstIt.previous()}           -> std::same_as<T>;
		{--it}                       -> std::same_as<const T&>;
		{it--}                       -> std::same_as<T>;
		{cstIt == cstIt}             -> std::same_as<bool>;
		{cstIt.getLinkedContainer()} -> se::iterator::IsPointer;
		*cstIt;
		cstIt.operator->();
	};

	template <typename T>
	concept TwoWayIterator = se::iterator::ForwardIterator<T> && se::iterator::BackwardIterator<T>;

	template <typename T>
	concept RandomAccessIterator = se::iterator::TwoWayIterator<T> && requires(T it, const T cstIt) {
		{it += se::Count()}   -> std::same_as<const T&>;
		{it -= se::Count()}   -> std::same_as<const T&>;
		{cstIt + se::Count()} -> std::same_as<T>;
		{se::Count() + cstIt} -> std::same_as<T>;
		{cstIt - se::Count()} -> std::same_as<T>;
		{cstIt - cstIt}       -> std::same_as<se::Count>;
	};

} // namespace se::iterator

#endif