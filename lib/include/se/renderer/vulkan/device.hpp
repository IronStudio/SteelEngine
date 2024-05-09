#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/gpuType.hpp"
#include "se/types.hpp"



namespace se::renderer::vulkan {
	struct DeviceInfos {
		VkInstance instance;
		se::renderer::GPUType gpuType;
	};


	class Device {
		public:
			Device(const se::renderer::vulkan::DeviceInfos &infos);
			~Device();

			inline explicit operator VkDevice() const noexcept {return m_device;}

		private:
			struct ScoreCriterias {
				se::renderer::GPUType gpuType;
			};

			static VkPhysicalDevice s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias);
			static se::Int32 s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias);
			static VkDevice s_createDevice(VkPhysicalDevice physicalDevice);

			se::renderer::vulkan::DeviceInfos m_infos;
			VkDevice m_device;
	};

} // namespace se::renderer::vulkan
