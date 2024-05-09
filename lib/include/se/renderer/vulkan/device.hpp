#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/renderer/gpuType.hpp"
#include "se/types.hpp"
#include "se/utils/bitField.hpp"



namespace se::renderer::vulkan {
	SE_CREATE_BIT_FIELD(QueueType, QueueTypeMask,
		ePresent   = 0b0000'0001,
		eGraphics  = 0b0000'0010,
		eCompute   = 0b0000'0100,
		eTransfer  = 0b0000'1000,
		eProtected = 0b0001'0000
	)

	se::renderer::vulkan::QueueType queueTypeVkToSe(VkQueueFlagBits queue);
	VkQueueFlagBits queueTypeSeToVk(se::renderer::vulkan::QueueType queue);


	struct DeviceInfos {
		VkInstance instance;
		se::renderer::GPUType gpuType;
		std::vector<const char*> extensions;
		se::renderer::vulkan::QueueTypeMask queueTypeMask;
	};


	class Device {
		public:
			Device(const se::renderer::vulkan::DeviceInfos &infos);
			~Device();

			inline VkDevice getDevice() const noexcept {return m_device;}

		private:
			struct ScoreCriterias {
				se::renderer::GPUType gpuType;
				std::vector<const char*> extensions;
				se::renderer::vulkan::QueueTypeMask queueTypeMask;
			};

			static VkPhysicalDevice s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias);
			static se::Int32 s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias);
			static VkDevice s_createDevice(VkPhysicalDevice physicalDevice, const std::vector<const char*> &extensions);
			static std::vector<VkQueue> s_getQueues(VkDevice device);

			se::renderer::vulkan::DeviceInfos m_infos;
			VkDevice m_device;
			std::vector<VkQueue> m_queues;
	};

} // namespace se::renderer::vulkan
