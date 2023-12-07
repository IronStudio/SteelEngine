#pragma once

#include <cinttypes>

#include "../core.hpp"



namespace se
{
	template <typename T>
	class SE_CORE PoolAllocator
	{
		public:
			class Handle final
			{
				friend class se::PoolAllocator<T>;

				public:
					Handle();
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




			PoolAllocator(size_t size);
			~PoolAllocator();

			bool isHandleValid(const se::PoolAllocator<T>::Handle &handle) const;

			se::PoolAllocator<T>::Handle allocate();

			inline void free(const se::PoolAllocator<T>::Handle &handle);

			inline bool isValid() const noexcept;
			inline size_t getSize() const noexcept;
			inline size_t getUsage() const noexcept;


		private:
			struct Data
			{
				T data;
				bool isInUse;
			};

			se::PoolAllocator<T>::Data *m_data;
			size_t m_size;
			size_t m_usage;
	};

} // namespace se


#include "poolAllocator.inl"