#pragma once

#include "../core.hpp"
#include "../essentials/intTypes.hpp"



namespace se
{
	/**
	 * @brief A pool allocator, use to allocate efficiently memory for a one-typed usage
	 * @tparam T The type of the pool
	*/
	template <typename T>
	class SE_CORE PoolAllocator
	{
		public:
			/**
			 * @brief A memory-safe handle to a `se::PoolAllocator<T>`
			*/
			class Handle final
			{
				friend class se::PoolAllocator<T>;

				public:
					Handle();
					/**
					 * @deprecated Please consider using the handle given by `se::PoolAllocator<T>.allocate()` or copying handle instead
					 *             of using raw pointer
					*/
					Handle(const se::PoolAllocator<T> *poolAllocator, T *ptr);
					Handle(const se::PoolAllocator<T>::Handle &handle);
					Handle(se::PoolAllocator<T>::Handle &&handle) noexcept;

					const se::PoolAllocator<T>::Handle &operator=(const se::PoolAllocator<T>::Handle &handle);
					const se::PoolAllocator<T>::Handle &operator=(se::PoolAllocator<T>::Handle &&handle) noexcept;

					T &operator*() const;

					[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
					T *getRawPointer() const;

					inline bool isValid() const noexcept;


				private:
					T *m_ptr;
					const se::PoolAllocator<T> *m_poolAllocator;
			};



			/**
			 * @param size The number of element of type `T` in the pool allocator
			*/
			PoolAllocator(se::Length size);
			~PoolAllocator();

			/**
			 * @brief Check if a handle is valid, that is his memory is still allocated and is owned by the allocator
			*/
			bool isHandleValid(const se::PoolAllocator<T>::Handle &handle) const;

			/**
			 * @brief Allocate one element of a pool allocator
			 * @warning Don't forget to free the given element
			*/
			se::PoolAllocator<T>::Handle allocate();

			/**
			 * @brief Free one element of a pool allocator
			 * @warning `handle` must be a valid element of the pool allocator
			*/
			inline void free(const se::PoolAllocator<T>::Handle &handle);

			inline bool isValid() const noexcept;
			/**
			 * @brief Get the maxium number of element in the pool allocator
			*/
			inline se::Length getSize() const noexcept;
			/**
			 * @brief Get the number of element currently allocated in the pool allocator
			*/
			inline se::Length getUsage() const noexcept;


		private:
			struct Data
			{
				T data;
				bool isInUse;
			};

			se::PoolAllocator<T>::Data *m_data;
			se::Length m_size;
			se::Length m_usage;
	};

} // namespace se


#include "poolAllocator.inl"