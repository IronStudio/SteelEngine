#pragma once

#include <cinttypes>

#include "../origin.hpp"



namespace se
{
	class SE_CORE StackAllocator final
	{
		public:
			template <typename T>
			class Handle final
			{
				friend class se::StackAllocator;

				public:
					Handle();
					Handle(const se::StackAllocator *stackAllocator, void *ptr);
					Handle(const se::StackAllocator::Handle<T> &handle);
					Handle(se::StackAllocator::Handle<T> &&handle) noexcept;

					const se::StackAllocator::Handle<T> &operator=(const se::StackAllocator::Handle<T> &handle);
					const se::StackAllocator::Handle<T> &operator=(se::StackAllocator::Handle<T> &&handle) noexcept;

					T &operator*() const;

					[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
					T *getRawPointer() const;

					inline bool isValid() const noexcept;


				private:
					void *m_ptr;
					const se::StackAllocator *m_stackAllocator;
			};


			using Point = Handle<uint8_t>;


			StackAllocator(size_t size);
			~StackAllocator();

			[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
			bool isPointerValid(void *ptr) const;
			template <typename T>
			bool isHandleValid(const se::StackAllocator::Handle<T> &handle) const;

			inline se::StackAllocator::Point getStackTop() const noexcept;

			template <typename T>
			se::StackAllocator::Handle<T> allocate(size_t amount = 1);

			inline void free(const se::StackAllocator::Point &point);

			inline bool isValid() const noexcept;
			inline size_t getSize() const noexcept;
			inline size_t getUsage() const noexcept;



		private:
			void *m_stackBottom;
			void *m_stackTop;
			size_t m_stackSize;
	};

} // namespace se



#include "stackAllocator.inl"