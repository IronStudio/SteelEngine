#pragma once

#include "se/core.hpp"
#include "se/uuid.hpp"



namespace se::memory
{
	template <typename T>
	class PoolAllocator;



	template <typename T>
	class PoolHandle final {
		friend class se::memory::PoolAllocator<T>;

		public:
			inline PoolHandle();
			inline ~PoolHandle() = default;

			inline PoolHandle(const se::memory::PoolHandle<T> &handle);
			inline const se::memory::PoolHandle<T> &operator=(const se::memory::PoolHandle<T> &handle);

			inline T &operator*() const;
			inline T *operator->() const;
			inline T &operator[](se::Size index) const;

			inline bool operator==(const se::memory::PoolHandle<T> &handle) const;
			inline explicit operator bool() const;
			inline explicit operator std::string() const;
			inline explicit operator T*() const;

			inline se::Size getCount() const noexcept;


		protected:
			inline PoolHandle(se::memory::PoolAllocator<T> *allocator, se::Count index, se::Count count);

		private:
			se::memory::PoolAllocator<T> *m_allocator;
			se::Count m_index;
			se::Count m_count;
	};


	template <typename T>
	inline std::ostream &operator<<(std::ostream &stream, const se::memory::PoolHandle<T> &handle);



	template <typename T>
	class SE_CORE PoolAllocator final {
		friend class se::memory::PoolHandle<T>;

		public:
			using Handle = se::memory::PoolHandle<T>;

			PoolAllocator();
			PoolAllocator(se::Size size);
			~PoolAllocator();

			PoolAllocator(const se::memory::PoolAllocator<T> &) = delete;
			const se::memory::PoolAllocator<T> &operator=(const se::memory::PoolAllocator<T> &) = delete;

			PoolAllocator(se::memory::PoolAllocator<T> &&allocator) noexcept;
			const se::memory::PoolAllocator<T> &operator=(se::memory::PoolAllocator<T> &&allocator) noexcept;

			template <bool throwOnAllocationFailure = true>
			se::memory::PoolAllocator<T>::Handle allocate(se::Count count);
			void free(const se::memory::PoolAllocator<T>::Handle &handle);
			template <bool throwOnAllocationFailure = true>
			se::memory::PoolAllocator<T>::Handle reallocate(const se::memory::PoolAllocator<T>::Handle &handle, se::Size count);

			void resize(se::Size size);


		protected:
			inline T *m_getElementPointer(se::Count index);

		private:
			se::Size m_size;
			T *m_data;
			bool *m_usedSpace;

		#undef A
	};



} // namespace se::memory



#include "se/memory/poolAllocator.inl"