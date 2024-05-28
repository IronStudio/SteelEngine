#pragma once

#include <map>
#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se::renderer::vulkan {
	class SE_CORE VramAllocatorHandle : public se::renderer::VramAllocatorHandle {
		public:
			VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount offset, VkDeviceMemory memory);
			~VramAllocatorHandle() override;
			bool isValid() override;

			inline se::ByteCount getOffset() const noexcept {return m_offset;}
			inline VkDeviceMemory getMemory() const noexcept {return m_memory;}


		private:
			se::ByteCount m_offset;
			VkDeviceMemory m_memory;
	};


	class SE_CORE VramAllocator : public se::renderer::VramAllocator {
		public:
			VramAllocator(const se::renderer::VramAllocatorInfos &infos);
			~VramAllocator() override;

			std::unique_ptr<Handle> allocate(const se::renderer::VramAllocationInfos &allocationInfos) override;
			void free(Handle *handle) override;

			void logAllocationTable() const noexcept;


		private:
			struct AllocationTableEntry {
				se::ByteCount start;
				se::ByteCount end;
			};

			static VkDeviceMemory s_allocateDeviceMemory(const se::renderer::VramAllocatorInfos &infos);
			static se::Int32 s_scoreMemoryType(se::renderer::VramUsageNature usage, VkMemoryPropertyFlags flags);

			std::vector<VkDeviceMemory> m_memories;
			std::map<VkDeviceMemory, std::vector<AllocationTableEntry>> m_allocationTables;
	};

} // namespace se::renderer::vulkan
