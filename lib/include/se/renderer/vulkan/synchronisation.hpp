#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/duration.hpp"
#include "se/renderer/context.hpp"



namespace se::renderer::vulkan {
	struct SemaphoreInfos {
		se::renderer::Context *context;
	};

	class SE_CORE Semaphore final {
		public:
			Semaphore();
			Semaphore(const se::renderer::vulkan::SemaphoreInfos &infos);
			~Semaphore();

			Semaphore(se::renderer::vulkan::Semaphore &&semaphore);
			const se::renderer::vulkan::Semaphore &operator=(se::renderer::vulkan::Semaphore &&semaphore);

			Semaphore(const se::renderer::vulkan::Semaphore &) = delete;
			const se::renderer::vulkan::Semaphore &operator=(const se::renderer::vulkan::Semaphore &) = delete;

			inline const VkSemaphore &getSemaphore() const noexcept {return m_semaphore;}

		private:
			se::renderer::vulkan::SemaphoreInfos m_infos;
			VkSemaphore m_semaphore;
	};



	struct FenceInfos {
		se::renderer::Context *context;
		bool signaled {false};
	};

	enum class SyncState {
		eSuccess,
		eTimeout
	};

	#define SE_NEVER_TIMEOUT se::Nanosecond(INFINITY)

	class SE_CORE Fence final {
		public:
			Fence();
			Fence(const se::renderer::vulkan::FenceInfos &infos);
			~Fence();

			Fence(se::renderer::vulkan::Fence &&fence);
			const se::renderer::vulkan::Fence &operator=(se::renderer::vulkan::Fence &&fence);

			Fence(const se::renderer::vulkan::Fence &) = delete;
			const se::renderer::vulkan::Fence &operator=(const se::renderer::vulkan::Fence &) = delete;

			se::renderer::vulkan::SyncState sync(se::Nanosecond timeout);
			void reset();

			inline const VkFence &getFence() const noexcept {return m_fence;}

			static SyncState sync(se::Nanosecond timeout, bool waitForAll, const std::vector<Fence*> &fences);
			static void reset(const std::vector<Fence*> &fences);

		private:
			se::renderer::vulkan::FenceInfos m_infos;
			VkFence m_fence;
	};

} // namespace se::renderer::vulkan
