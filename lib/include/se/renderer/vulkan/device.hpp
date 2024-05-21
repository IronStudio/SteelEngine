#pragma once

#include <map>
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

	#define SE_QUEUE_TYPE_COUNT 5

	se::renderer::vulkan::QueueType queueTypeVkToSe(VkQueueFlagBits queue);
	VkQueueFlagBits queueTypeSeToVk(se::renderer::vulkan::QueueType queue);
	se::renderer::vulkan::QueueTypeMask queueTypeMaskVkToSe(VkQueueFlags queue);
	VkQueueFlags queueTypeMaskSeToVk(se::renderer::vulkan::QueueTypeMask queue);


	struct DeviceInfos {
		VkInstance instance;
		se::renderer::GPUType gpuType;
		std::vector<const char*> extensions;
		se::renderer::vulkan::QueueTypeMask queueTypeMask;
		VkSurfaceKHR surface;
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
				VkSurfaceKHR surface;
			};

			struct DeviceCreateInfos {
				VkPhysicalDevice device;
				std::vector<const char*> extensions;
				se::renderer::vulkan::QueueTypeMask queueTypeMask;
			};

			struct QueueInfos {
				QueueTypeMask type;
				se::Count count;
			};

			static VkPhysicalDevice s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias);
			static se::Int32 s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias);
			static VkDevice s_createDevice(const DeviceCreateInfos &infos, std::map<QueueType, std::vector<VkQueue>> &queues);
			static std::map<QueueType, std::vector<VkQueue>> s_getQueues(VkDevice device, const std::vector<QueueInfos> &queues);

			se::renderer::vulkan::DeviceInfos m_infos;
			VkDevice m_device;
			std::map<QueueType, std::vector<VkQueue>> m_queues;
	};

} // namespace se::renderer::vulkan
