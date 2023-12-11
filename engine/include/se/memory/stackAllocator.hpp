#pragma once

#include "../essentials/intTypes.hpp"
#include "../origin.hpp"



namespace se
{
	/**
	 * @brief A pool allocator, use to allocate efficiently memory that is context-dependent
	 * @tparam T The type of the pool
	*/
	class SE_CORE StackAllocator final
	{
		public:
			/**
			 * @brief A memory-safe handle to a `se::StackAllocator`
			 * @tparam T The type of the data contained in the handle
			*/
			template <typename T>
			class Handle final
			{
				friend class se::StackAllocator;

				public:
					Handle();
					/**
					 * @deprecated Please consider using the handle given by `se::StackAllocator.allocate<T> ()`
					 *             or copying handle instead of using raw pointer
					*/
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


			/**
			 * @brief A type use to save a point in the stack, so you can free it later
			*/
			using Point = Handle<se::Uint8>;


			/**
			 * @param size The size of the stack in bytes
			*/
			StackAllocator(se::Size size);
			~StackAllocator();

			[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
			bool isPointerValid(void *ptr) const;
			/**
			 * @brief Check if a handle is valid, that is his memory is still allocated and is owned by the allocator
			*/
			template <typename T>
			bool isHandleValid(const se::StackAllocator::Handle<T> &handle) const;

			/**
			 * @brief Get the current stack top to be able to free the stack later
			*/
			inline se::StackAllocator::Point getStackTop() const noexcept;

			/**
			 * @brief Allocate memory for `amount` element of type `T` on the stack
			 * @param amount The amount of element to allocate. Default to `1`
			 * @tparam T The type of the data to allocate
			 * @warning Please don't forget to save stack top with `se::StackAllocator.getStackTop()` to later free the stack
			*/
			template <typename T>
			se::StackAllocator::Handle<T> allocate(se::Size amount = 1);

			/**
			 * @brief Reverse the stack to `point`, that is deallocate memory from the current stack top to `point`
			 * @warning `point` must be **below** the stack top
			*/
			inline void free(const se::StackAllocator::Point &point);

			inline bool isValid() const noexcept;
			/**
			 * @brief Get stack max size in bytes
			*/
			inline se::Size getSize() const noexcept;
			/**
			 * @brief Get stack currently allocated size in bytes
			*/
			inline se::Size getUsage() const noexcept;



		private:
			void *m_stackBottom;
			void *m_stackTop;
			se::Size m_stackSize;
	};

} // namespace se



#include "stackAllocator.inl"