#include "se/renderer/vulkan/device.hpp"

#include <cstring>
#include <iostream>
#include <vector>

#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	Device::Device(const se::renderer::vulkan::DeviceInfos &infos) :
		m_infos {infos},
		m_device {VK_NULL_HANDLE}
	{
		ScoreCriterias scoreCriterias {};
		scoreCriterias.gpuType = m_infos.gpuType;

		VkPhysicalDevice physicalDevice {s_chooseDevice(m_infos.instance, scoreCriterias)};
		m_device = s_createDevice(physicalDevice);
	}



	Device::~Device() {
		if (m_device != VK_NULL_HANDLE)
			vkDestroyDevice(m_device, nullptr);
	}



	VkPhysicalDevice Device::s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias) {
		se::Uint32 count{};
		if (vkEnumeratePhysicalDevices(instance, &count, nullptr) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get physical devices count");

		std::vector<VkPhysicalDevice> devices {};
		devices.resize(count);
		if (vkEnumeratePhysicalDevices(instance, &count, devices.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get physical devices");

		VkPhysicalDevice bestDevice{VK_NULL_HANDLE};
		se::Int32 bestScore{-1};
		for (const auto &device : devices)
		{
			se::Int32 score{s_scoreDevice(device, criterias)};
			if (score <= bestScore)
				continue;
			bestScore = score;
			bestDevice = device;
		}

		if (bestScore < 0 || bestDevice == VK_NULL_HANDLE)
			throw se::exceptions::BadAllocation("No GPU can be used by SteelEngine");

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(bestDevice, &properties);

		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo);
		SE_LOGGER << "Selected device :\n";
		SE_LOGGER << "\tName               : " << properties.deviceName << "\n";
		SE_LOGGER << "\tVulkan API version : " << VK_API_VERSION_MAJOR(properties.apiVersion)
				  << "." << VK_API_VERSION_MINOR(properties.apiVersion) << VK_API_VERSION_PATCH(properties.apiVersion) << "\n";
		SE_LOGGER << "\tType               : " << (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "intergrated" : "discrete") << "\n";
		SE_LOGGER << se::endLog;

		return bestDevice;
	}



	se::Int32 Device::s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias) {
		se::Int32 score{0};

		VkPhysicalDeviceProperties properties{};
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceProperties(device, &properties);
		vkGetPhysicalDeviceFeatures(device, &features);

		if (criterias.gpuType == se::renderer::GPUType::eDiscret && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;
		else if (criterias.gpuType == se::renderer::GPUType::eDiscret && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			score += 1000;

		return score;
	}



	VkDevice Device::s_createDevice(VkPhysicalDevice physicalDevice) {
		VkDevice device{};

		VkPhysicalDeviceFeatures features{};

		VkDeviceCreateInfo deviceCreateInfos{};
		deviceCreateInfos.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfos.enabledExtensionCount = 0;
		deviceCreateInfos.ppEnabledExtensionNames = nullptr;
		deviceCreateInfos.enabledLayerCount = 0;
		deviceCreateInfos.ppEnabledLayerNames = nullptr;
		deviceCreateInfos.pEnabledFeatures = &features;
		deviceCreateInfos.queueCreateInfoCount = 0;
		deviceCreateInfos.pQueueCreateInfos = nullptr;

		if (vkCreateDevice(physicalDevice, &deviceCreateInfos, nullptr, &device) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create logical device");

		return device;
	}

} // namespace se::renderer::vulkan
