#include "se/renderer/vulkan/vramAllocator.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {
	VramAllocatorHandle::VramAllocatorHandle() :
		se::renderer::VramAllocatorHandle(nullptr),
		m_offset {0}
	{

	}


	VramAllocatorHandle::~VramAllocatorHandle() {

	}


	VramAllocatorHandle::VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount offset) :
		se::renderer::VramAllocatorHandle(allocator),
		m_offset {offset}
	{

	}




	VramAllocator::VramAllocator(const se::renderer::VramAllocatorInfos &infos) :
		se::renderer::VramAllocator(infos),
		m_memory {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		VkPhysicalDevice physicalDevice {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getPhysicalDevice()};

		VkPhysicalDeviceMemoryProperties memoryProperties {};
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		VkMemoryAllocateInfo allocateInfos {};
		allocateInfos.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfos.memoryTypeIndex = 0;
		allocateInfos.allocationSize = m_infos.chunkSize;

		if (vkAllocateMemory(device, &allocateInfos, nullptr, &m_memory) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't allocate memory");
	}


	VramAllocator::~VramAllocator() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_memory != VK_NULL_HANDLE)
			vkFreeMemory(device, m_memory, nullptr);
	}


	std::unique_ptr<VramAllocator::Handle> VramAllocator::allocate(const se::renderer::VramAllocationInfos &allocationInfos) {

	}


	void VramAllocator::free(Handle *handle) {

	}




} // namespace se::renderer::vulkan
