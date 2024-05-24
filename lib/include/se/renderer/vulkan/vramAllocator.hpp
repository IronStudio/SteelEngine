#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se::renderer::vulkan {
	struct VramAllocator;

	class SE_CORE VramAllocatorHandle : public se::renderer::VramAllocatorHandle {
		friend class se::renderer::vulkan::VramAllocator;

		public:
			VramAllocatorHandle();
			~VramAllocatorHandle() override;

			inline se::ByteCount getOffset() const noexcept {return m_offset;}


		private:
			VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount offset);

			se::ByteCount m_offset;
	};


	class SE_CORE VramAllocator : public se::renderer::VramAllocator {
		public:
			VramAllocator(const se::renderer::VramAllocatorInfos &infos);
			~VramAllocator() override;

			std::unique_ptr<Handle> allocate(const se::renderer::VramAllocationInfos &allocationInfos) override;
			void free(Handle *handle) override;


		private:
			VkDeviceMemory m_memory;
	};

} // namespace se::renderer::vulkan
