#pragma once

#include <limits>

#include "../core.hpp"
#include "../essentials/intTypes.hpp"



namespace se
{
	class SE_CORE HeapAllocator final
	{
		public:
			/**
			 * @brief A memory-safe handle to a `se::HeapAllocator`
			 * @tparam T The type of the data contained in the handle
			*/
			template <typename T>
			class Handle final
			{
				friend class se::HeapAllocator;

				public:
					Handle();
					/**
					 * @deprecated Please consider using the handle given by `se::HeapAllocator.allocate<T> ()`
					 *             or copying handle instead of using raw pointer
					*/
					Handle(const se::HeapAllocator *heapAllocator, void *ptr, se::Length count = 1);
					Handle(const se::HeapAllocator::Handle<T> &handle);
					Handle(se::HeapAllocator::Handle<T> &&handle) noexcept;

					const se::HeapAllocator::Handle<T> &operator=(const se::HeapAllocator::Handle<T> &handle);
					const se::HeapAllocator::Handle<T> &operator=(se::HeapAllocator::Handle<T> &&handle) noexcept;

					T &operator*() const;

					[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
					T *getRawPointer() const;

					inline bool isValid() const noexcept;


				private:
					void *m_ptr;
					const se::HeapAllocator *m_heapAllocator;
					se::Length m_count;
			};



			/**
			 * @brief Create a heap allocator
			 * @param size The size of the allocator, in bytes
			 * @param ratio A ratio use to create reference table.
			 *              Set it to 1 if all your data will be 1 bytes wide, 0.5 for 2 bytes wide, etc
			 * @warning `ratio` must be in ]0; 1]
			*/
			HeapAllocator(se::Size size, se::Float ratio = 0.5f);
			~HeapAllocator();

			/**
			 * @brief Check if a handle is valid, that is his memory is still allocated and is owned by the allocator
			*/
			template <typename T>
			bool isHandleValid(const se::HeapAllocator::Handle<T> &handle) const;

			/**
			 * @brief Allocate memory for `amount` element of type `T` on the stack
			 * @param amount The amount of element to allocate. Default to `1`
			 * @tparam T The type of the data to allocate
			 * @warning Please don't forget to save stack top with `se::StackAllocator.getStackTop()` to later free the stack
			*/
			template <typename T>
			se::HeapAllocator::Handle<T> allocate(se::Length amount = 1);
			template <typename T>
			void free(const se::HeapAllocator::Handle<T> &handle);

			/**
			 * @brief Remove free space accumulated between alloacted block
			 * @param maxChunkCount The maximum number of block to move. Use it to spreed defragmentation across time
			 * @warning Not implemented yet
			*/
			void defragment(se::Length maxChunkCount = std::numeric_limits<se::Length>::max());


			inline bool isValid() const noexcept;
			/**
			 * @brief Get heap max size in bytes
			*/
			inline se::Size getSize() const noexcept;
			/**
			 * @brief Get heap currently allocated size in bytes
			*/
			inline se::Size getUsage() const noexcept;

			/**
			 * @brief Get the number of free space between allocated blocks, or `0` if there is no fragmentation
			*/
			se::Size getFragmentationSpaceSize() const noexcept;


		private:
			struct ReferenceTableElement
			{
				void *ptr;
				se::Size size;
				bool isUsed;
			};

			void *m_heap;
			void *m_nextBlockToAllocate;
			se::HeapAllocator::ReferenceTableElement *m_referenceTable;
			se::Size m_size;
			se::Size m_usage;
			se::Length m_referenceTableSize;
	};



} // namespace se



#include "heapAllocator.inl"