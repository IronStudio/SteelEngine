#pragma once

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
			inline PoolHandle(T *ptr, se::Size count);

		private:
			T *m_ptr;
			se::Size m_count;
	};


	template <typename T>
	inline std::ostream &operator<<(std::ostream &stream, const se::memory::PoolHandle<T> &handle);



	template <typename T>
	class PoolAllocator final {
		public:
			using Handle = se::memory::PoolHandle<T>;

			PoolAllocator();
			PoolAllocator(se::Size size);
			~PoolAllocator();

			PoolAllocator(const se::memory::PoolAllocator<T> &) = delete;
			const se::memory::PoolAllocator<T> &operator=(const se::memory::PoolAllocator<T> &) = delete;

			PoolAllocator(se::memory::PoolAllocator<T> &&allocator) noexcept;
			const se::memory::PoolAllocator<T> &operator=(se::memory::PoolAllocator<T> &&allocator) noexcept;

			se::memory::PoolAllocator<T>::Handle allocate(se::Size count);
			void free(const se::memory::PoolAllocator<T>::Handle &handle);
			se::memory::PoolAllocator<T>::Handle reallocate(const se::memory::PoolAllocator<T>::Handle &handle, se::Size count);



		private:
			se::Size m_size;
			T *m_data;
			bool *m_usedSpace;
	};



} // namespace se::memory



#include "se/memory/poolAllocator.inl"