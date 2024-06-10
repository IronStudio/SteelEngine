#include "se/renderer/vulkan/synchronisation.hpp"

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {
	Semaphore::Semaphore() :
		m_infos {},
		m_semaphore {VK_NULL_HANDLE}
	{

	}


	Semaphore::Semaphore(const se::renderer::vulkan::SemaphoreInfos &infos) :
		m_infos {infos},
		m_semaphore {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		VkSemaphoreCreateInfo semaphoreCreateInfos {};
		semaphoreCreateInfos.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		if (vkCreateSemaphore(device, &semaphoreCreateInfos, nullptr, &m_semaphore) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create semaphore");
	}


	Semaphore::~Semaphore() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		if (m_semaphore != VK_NULL_HANDLE)
			vkDestroySemaphore(device, m_semaphore, nullptr);
	}


	Semaphore::Semaphore(se::renderer::vulkan::Semaphore &&semaphore) :
		m_infos {semaphore.m_infos},
		m_semaphore {semaphore.m_semaphore}
	{
		semaphore.m_semaphore = VK_NULL_HANDLE;
	}


	const se::renderer::vulkan::Semaphore &Semaphore::operator=(se::renderer::vulkan::Semaphore &&semaphore) {
		m_infos = semaphore.m_infos;
		m_semaphore = semaphore.m_semaphore;
		semaphore.m_semaphore = VK_NULL_HANDLE;
		return *this;
	}




	Fence::Fence() :
		m_infos {},
		m_fence {VK_NULL_HANDLE}
	{

	}


	Fence::Fence(const se::renderer::vulkan::FenceInfos &infos) :
		m_infos {infos},
		m_fence {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		VkFenceCreateInfo fenceCreateInfos {};
		fenceCreateInfos.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if (m_infos.signaled)
			fenceCreateInfos.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateFence(device, &fenceCreateInfos, nullptr, &m_fence) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create fence");
	}


	Fence::~Fence() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		if (m_fence != VK_NULL_HANDLE)
			vkDestroyFence(device, m_fence, nullptr);
	}


	Fence::Fence(se::renderer::vulkan::Fence &&semaphore) :
		m_infos {semaphore.m_infos},
		m_fence {semaphore.m_fence}
	{
		semaphore.m_fence = VK_NULL_HANDLE;
	}


	const se::renderer::vulkan::Fence &Fence::operator=(se::renderer::vulkan::Fence &&semaphore) {
		m_infos = semaphore.m_infos;
		m_fence = semaphore.m_fence;
		semaphore.m_fence = VK_NULL_HANDLE;
		return *this;
	}


	se::renderer::vulkan::SyncState Fence::sync(se::Nanosecond timeout) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		se::Uint64 timeoutTime {static_cast<se::Uint64> (se::Duration<se::Uint64, se::Nano> (timeout))};
		if (timeout == SE_NEVER_TIMEOUT)
			timeoutTime = UINT64_MAX;

		VkResult result  {vkWaitForFences(device, 1, &m_fence, VK_TRUE, timeoutTime)};

		if (result == VK_TIMEOUT)
			return se::renderer::vulkan::SyncState::eTimeout;
		if (result == VK_SUCCESS)
			return se::renderer::vulkan::SyncState::eSuccess;
		throw se::exceptions::RuntimeError("Can't wait for fence");
	}


	void Fence::reset() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		if (vkResetFences(device, 1, &m_fence) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't reset fence");
	}


	SyncState Fence::sync(se::Nanosecond timeout, bool waitForAll, const std::vector<Fence*> &fences) {
		SE_ASSERT(!fences.empty(), "Can't sync fences if no fences were given");
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (fences[0]->m_infos.context)->getDevice()->getDevice()};

		std::vector<VkFence> vulkanFences {};
		vulkanFences.reserve(fences.size());
		for (const auto &fence : fences)
			vulkanFences.push_back(fence->getFence());

		se::Uint64 timeoutTime {static_cast<se::Uint64> (se::Duration<se::Uint64, se::Nano> (timeout))};
		if (timeout == SE_NEVER_TIMEOUT)
			timeoutTime = UINT64_MAX;

		VkResult result  {vkWaitForFences(device, vulkanFences.size(), vulkanFences.data(), waitForAll, timeoutTime)};

		if (result == VK_TIMEOUT)
			return se::renderer::vulkan::SyncState::eTimeout;
		if (result == VK_SUCCESS)
			return se::renderer::vulkan::SyncState::eSuccess;
		throw se::exceptions::RuntimeError("Can't wait for fences");
	}


	void Fence::reset(const std::vector<Fence*> &fences) {
		SE_ASSERT(!fences.empty(), "Can't sync fences if no fences were given");
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (fences[0]->m_infos.context)->getDevice()->getDevice()};

		std::vector<VkFence> vulkanFences {};
		vulkanFences.reserve(fences.size());
		for (const auto &fence : fences)
			vulkanFences.push_back(fence->getFence());

		if (vkResetFences(device, vulkanFences.size(), vulkanFences.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't reset fences");
	}


} // namespace se::renderer::vulkan
