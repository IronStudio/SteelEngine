#pragma once

#include <map>
#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/gpuType.hpp"
#include "se/types.hpp"
#include "se/utils/bitField.hpp"



namespace se::renderer::vulkan {
	SE_CREATE_BIT_FIELD(QueueType, QueueTypeMask,
		eGraphics  = 0b0000'0001,
		ePresent   = 0b0000'0010,
		eCompute   = 0b0000'0100,
		eTransfer  = 0b0000'1000,
		eProtected = 0b0001'0000
	)

	#define SE_QUEUE_TYPE_COUNT 5

	SE_CORE se::renderer::vulkan::QueueType queueTypeVkToSe(VkQueueFlagBits queue);
	SE_CORE VkQueueFlagBits queueTypeSeToVk(se::renderer::vulkan::QueueType queue);
	SE_CORE se::renderer::vulkan::QueueTypeMask queueTypeMaskVkToSe(VkQueueFlags queue);
	SE_CORE VkQueueFlags queueTypeMaskSeToVk(se::renderer::vulkan::QueueTypeMask queue);


	struct DeviceInfos {
		VkInstance instance;
		se::renderer::GPUType gpuType;
		std::vector<const char*> extensions;
		se::renderer::vulkan::QueueTypeMask queueTypeMask;
		VkSurfaceKHR surface;
		VkPhysicalDeviceFeatures2 requiredFeatures;
	};


	class SE_CORE Device {
		public:
			Device(const se::renderer::vulkan::DeviceInfos &infos);
			~Device();

			inline VkDevice getDevice() const noexcept {return m_device;}
			inline VkPhysicalDevice getPhysicalDevice() const noexcept {return m_physicalDevice;}
			inline const std::map<QueueType, std::map<se::Count, std::vector<VkQueue>>> &getQueues() const noexcept {return m_queues;}
			inline const std::map<QueueType, se::Count> &getQueueFamilyIndices() const noexcept {return m_queueFamilyIndices;}
			VkQueue getQueue(QueueType queueType) const;

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
				VkSurfaceKHR surface;
				VkPhysicalDeviceFeatures2 features;
			};

			struct QueueInfos {
				QueueTypeMask type;
				se::Count count;
			};

			static VkPhysicalDevice s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias);
			static se::Int32 s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias);
			static VkDevice s_createDevice(const DeviceCreateInfos &infos, std::map<QueueType, std::map<se::Count, std::vector<VkQueue>>> &queues);
			static std::map<QueueType, std::map<se::Count, std::vector<VkQueue>>> s_getQueues(VkDevice device, const std::vector<QueueInfos> &queues);
			static std::map<QueueType, se::Count> s_dispatchQueueFamilies(const std::map<QueueType, std::map<se::Count, std::vector<VkQueue>>> &queues);

			se::renderer::vulkan::DeviceInfos m_infos;
			VkDevice m_device;
			VkPhysicalDevice m_physicalDevice;
			std::map<QueueType, std::map<se::Count, std::vector<VkQueue>>> m_queues;
			std::map<QueueType, se::Count> m_queueFamilyIndices;
	};

} // namespace se::renderer::vulkan
