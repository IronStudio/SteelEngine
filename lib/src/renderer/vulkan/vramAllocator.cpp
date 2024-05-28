#include "se/renderer/vulkan/vramAllocator.hpp"

#include <map>
#include <vector>

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {
	VramAllocatorHandle::VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount offset, VkDeviceMemory memory) :
		se::renderer::VramAllocatorHandle(allocator),
		m_offset {offset},
		m_memory {memory}
	{

	}


	VramAllocatorHandle::~VramAllocatorHandle() {

	}


	bool VramAllocatorHandle::isValid() {
		return m_memory != VK_NULL_HANDLE;
	}




	VramAllocator::VramAllocator(const se::renderer::VramAllocatorInfos &infos) :
		se::renderer::VramAllocator(infos),
		m_memories {},
		m_allocationTables {}
	{
		m_memories.push_back(s_allocateDeviceMemory(m_infos));
	}


	VramAllocator::~VramAllocator() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		for (auto &memory : m_memories) {
			if (memory != VK_NULL_HANDLE)
				vkFreeMemory(device, memory, nullptr);
		}
	}


	std::unique_ptr<VramAllocator::Handle> VramAllocator::allocate(const se::renderer::VramAllocationInfos &allocationInfos) {
		se::ByteCount offset {};
		VkDeviceMemory memory {VK_NULL_HANDLE};

		for (auto &deviceMemory : m_memories) {
			auto &allocationTable {m_allocationTables[deviceMemory]};
			se::ByteCount lastEnd {0};
			for (auto entry {allocationTable.begin()}; entry != allocationTable.end(); ++entry) {
				if (entry->start - lastEnd >= allocationInfos.size) {
					offset = lastEnd;
					memory = deviceMemory;
					allocationTable.insert(entry, {offset, offset + allocationInfos.size});
					goto allocate_ret;
				}

				lastEnd = entry->end;
				se:ByteCount alignementOffset {lastEnd % allocationInfos.alignement};
				if (alignementOffset != 0)
					lastEnd += allocationInfos.alignement - alignementOffset;
			}

			if (m_infos.chunkSize - lastEnd >= allocationInfos.size) {
				offset = lastEnd;
				memory = deviceMemory;
				allocationTable.push_back({offset, offset + allocationInfos.size});
				goto allocate_ret;
			}
		}

		m_memories.push_back(s_allocateDeviceMemory(m_infos));
		m_allocationTables[*m_memories.rbegin()] = {{0, allocationInfos.size}};
		offset = 0;
		memory = *m_memories.rbegin();

	allocate_ret:
		return std::make_unique<se::renderer::vulkan::VramAllocatorHandle> (this, offset, memory);
	}


	void VramAllocator::free(Handle *argHandle) {
		SE_ASSERT(argHandle->isValid(), "Can't free handle that is not valid");
		SE_ASSERT(argHandle->getAllocator() == this, "Can't free handle that is not linked to the current allocator");

		auto handle {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (argHandle)};
		auto &allocationTable {m_allocationTables[handle->getMemory()]};

		for (auto entry {allocationTable.begin()}; entry != allocationTable.end(); ++entry) {
			if (entry->start != handle->getOffset())
				continue;

			allocationTable.erase(entry);
			return;
		}

		SE_WARNING(
			"You attempted to free VramAllocator handle that does not exist inside the allocator (handle {} with offset {} and memory {})",
			(void*)handle, handle->getOffset(), (void*)handle->getMemory()
		);
	}


	void VramAllocator::logAllocationTable() const noexcept {
		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Logging allocation table of VramAllocator " << this << " with chunk size "
			<< m_infos.chunkSize << "B :\n";

		for (const auto &table : m_allocationTables) {
			SE_LOGGER << "\tMemory " << table.first << " :\n";
			for (const auto &entry : table.second)
				SE_LOGGER << "\t\t- start : " << entry.start << "B, end : " << entry.end << "B, size : " << entry.end - entry.start << "B\n";
		}

		SE_LOGGER << se::endLog;
	}


	VkDeviceMemory VramAllocator::s_allocateDeviceMemory(const se::renderer::VramAllocatorInfos &infos) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};
		VkPhysicalDevice physicalDevice {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getPhysicalDevice()};

		VkPhysicalDeviceMemoryProperties memoryProperties {};
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		std::vector<se::Int32> scoredMemoryTypes {};
		scoredMemoryTypes.reserve(memoryProperties.memoryTypeCount);
		for (se::Count i {0}; i < memoryProperties.memoryTypeCount; ++i)
			scoredMemoryTypes.push_back(s_scoreMemoryType(infos.usageNature, memoryProperties.memoryTypes[i].propertyFlags));

		se::Uint32 memoryTypeIndex {};
		se::Int32 bestScore {-1};
		for (se::Uint32 i {0}; i < scoredMemoryTypes.size(); ++i) {
			if (scoredMemoryTypes[i] <= bestScore)
				continue;
			bestScore = scoredMemoryTypes[i];
			memoryTypeIndex = i;
		}

		if (bestScore < 0)
			throw se::exceptions::RuntimeError("Can't find memory type suitable for the vram allocator");

		VkMemoryAllocateInfo allocateInfos {};
		allocateInfos.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfos.memoryTypeIndex = memoryTypeIndex;
		allocateInfos.allocationSize = infos.chunkSize;

		VkDeviceMemory memory {VK_NULL_HANDLE};
		if (vkAllocateMemory(device, &allocateInfos, nullptr, &memory) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't allocate memory");

		return memory;
	}


	se::Int32 VramAllocator::s_scoreMemoryType(se::renderer::VramUsageNature usage, VkMemoryPropertyFlags flags) {
		static std::map<se::renderer::VramUsageNature, std::vector<VkMemoryPropertyFlags>> usageNatureToFlagsMap {
			{se::renderer::VramUsageNature::eGpuToCpu, {VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT}},
			{se::renderer::VramUsageNature::eGpuToGpu, {
				
			}}
		};

		se::Int32 score {-1};

		if (usage == se::renderer::VramUsageNature::eCpuToGpu) {
			if ((flags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
				== (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
				score = 100;
			else if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				score = 50;
		}

		else if (usage == se::renderer::VramUsageNature::eGpuToCpu) {
			if ((flags % (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
				== (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
				score = 100;
		}

		else if (usage == se::renderer::VramUsageNature::eGpuToGpu) {
			if (!(flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				score = 100;
			else if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				score = 50;
		}

		return score;
	}


} // namespace se::renderer::vulkan
