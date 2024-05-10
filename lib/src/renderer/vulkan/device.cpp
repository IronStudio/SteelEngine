#include "se/renderer/vulkan/device.hpp"

#include <cstring>
#include <iostream>
#include <vector>

#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	se::renderer::vulkan::QueueType queueTypeVkToSe(VkQueueFlagBits queue) {
		static const std::map<VkQueueFlagBits, se::renderer::vulkan::QueueType> convertMap {
			{VK_QUEUE_COMPUTE_BIT,   QueueType::eCompute},
			{VK_QUEUE_GRAPHICS_BIT,  QueueType::eGraphics},
			{VK_QUEUE_TRANSFER_BIT,  QueueType::eTransfer},
			{VK_QUEUE_PROTECTED_BIT, QueueType::eProtected}
		};

		return convertMap.find(queue)->second;
	}

	VkQueueFlagBits queueTypeSeToVk(se::renderer::vulkan::QueueType queue) {
		static const std::map<se::renderer::vulkan::QueueType, VkQueueFlagBits> convertMap {
			{QueueType::eCompute,   VK_QUEUE_COMPUTE_BIT},
			{QueueType::eGraphics,  VK_QUEUE_GRAPHICS_BIT},
			{QueueType::eTransfer,  VK_QUEUE_TRANSFER_BIT},
			{QueueType::eProtected, VK_QUEUE_PROTECTED_BIT}
		};

		return convertMap.find(queue)->second;
	}

	se::renderer::vulkan::QueueTypeMask queueTypeMaskVkToSe(VkQueueFlags queue) {
		se::renderer::vulkan::QueueTypeMask output {};
		if (queue & VK_QUEUE_GRAPHICS_BIT)
			output |= QueueType::eGraphics;
		if (queue & VK_QUEUE_COMPUTE_BIT)
			output |= QueueType::eCompute;
		if (queue & VK_QUEUE_TRANSFER_BIT)
			output |= QueueType::eTransfer;
		if (queue & VK_QUEUE_PROTECTED_BIT)
			output |= QueueType::eProtected;
		return output;
	}

	VkQueueFlags queueTypeMaskVkToSe(se::renderer::vulkan::QueueTypeMask queue) {
		VkQueueFlags output {};
		if (queue & QueueType::eGraphics)
			output |= VK_QUEUE_GRAPHICS_BIT;
		if (queue & QueueType::eCompute)
			output |= VK_QUEUE_COMPUTE_BIT;
		if (queue & QueueType::eTransfer)
			output |= VK_QUEUE_TRANSFER_BIT;
		if (queue & QueueType::eProtected)
			output |= VK_QUEUE_PROTECTED_BIT;
		return output;
	}



	Device::Device(const se::renderer::vulkan::DeviceInfos &infos) :
		m_infos {infos},
		m_device {VK_NULL_HANDLE},
		m_queues {}
	{
		ScoreCriterias scoreCriterias {};
		scoreCriterias.gpuType = m_infos.gpuType;
		scoreCriterias.extensions = m_infos.extensions;
		scoreCriterias.queueTypeMask = m_infos.queueTypeMask;

		DeviceCreateInfos deviceCreateInfos {};
		deviceCreateInfos.device = s_chooseDevice(m_infos.instance, scoreCriterias);
		deviceCreateInfos.extensions = m_infos.extensions;
		deviceCreateInfos.queueTypeMask = m_infos.queueTypeMask;

		m_device = s_createDevice(deviceCreateInfos, m_queues);
	}



	Device::~Device() {
		if (m_device != VK_NULL_HANDLE)
			vkDestroyDevice(m_device, nullptr);
	}



	VkPhysicalDevice Device::s_chooseDevice(VkInstance instance, const ScoreCriterias &criterias) {
		se::Uint32 count {};
		if (vkEnumeratePhysicalDevices(instance, &count, nullptr) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get physical devices count");

		std::vector<VkPhysicalDevice> devices {};
		devices.resize(count);
		if (vkEnumeratePhysicalDevices(instance, &count, devices.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get physical devices");

		VkPhysicalDevice bestDevice{VK_NULL_HANDLE};
		se::Int32 bestScore {-1};
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

		VkPhysicalDeviceProperties properties {};
		vkGetPhysicalDeviceProperties(bestDevice, &properties);

		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo);
		SE_LOGGER << "Selected device :\n";
		SE_LOGGER << "\tName               : " << properties.deviceName << "\n";
		SE_LOGGER << "\tVulkan API version : " << VK_API_VERSION_MAJOR(properties.apiVersion)
				  << "." << VK_API_VERSION_MINOR(properties.apiVersion) << "." << VK_API_VERSION_PATCH(properties.apiVersion) << "\n";
		SE_LOGGER << "\tType               : " << (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "intergrated" : "discrete") << "\n";
		SE_LOGGER << se::endLog;

		return bestDevice;
	}



	se::Int32 Device::s_scoreDevice(VkPhysicalDevice device, const ScoreCriterias &criterias) {
		se::Int32 score{0};

		VkPhysicalDeviceProperties properties {};
		VkPhysicalDeviceFeatures features {};
		vkGetPhysicalDeviceProperties(device, &properties);
		vkGetPhysicalDeviceFeatures(device, &features);

		se::Uint32 queueCount {};
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies {};
		queueFamilies.resize(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());

		if (criterias.gpuType == se::renderer::GPUType::eDiscret && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;
		else if (criterias.gpuType == se::renderer::GPUType::eDiscret && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			score += 1000;

		se::renderer::vulkan::QueueTypeMask foundQueueTypes {};
		for (const auto &queue : queueFamilies) {
			foundQueueTypes |= queueTypeMaskVkToSe(queue.queueFlags)
			//vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &support);
		}

		if (criterias.queueTypeMask != (criterias.queueTypeMask & foundQueueTypes))
			return -1;

		return score;
	}


	VkDevice Device::s_createDevice(const DeviceCreateInfos &infos, std::map<QueueType, VkQueue> &queues) {
		VkDevice device {};

		VkPhysicalDeviceFeatures features {};

		se::Uint32 queueCount {};
		vkGetPhysicalDeviceQueueFamilyProperties(infos.device, &queueCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies {};
		queueFamilies.resize(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(infos.device, &queueCount, queueFamilies.data());

		std::vector<QueueInfos> availableQueuesInfos {};
		availableQueuesInfos.reserve(queueCount);

		for (const auto &queue : queueFamilies) {
			QueueInfos infos {};
			infos.type = queueTypeMaskVkToSe(queue.queueFlags);
			infos.count = queue.queueCount;
			availableQueuesInfos.push_back(infos);
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {s_chooseQueues(availableQueuesInfos, infos.queueTypeMask)};
		std::vector<std::vector<float>> priorities {};
		priorities.reserve(queueCreateInfos.size());
		for (auto &queue : queueCreateInfos) {
			priorities.push_back({});
			priorities.rbegin()->resize(queue.queueCount);
			for (se::Count i {0}; i < queue.queueCount; ++i)
				priorities.rbegin()->operator[](i) = 1.f;
			
			queue.pQueuePriorities = priorities.rbegin()->data();
		}

		VkDeviceCreateInfo deviceCreateInfos {};
		deviceCreateInfos.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfos.enabledExtensionCount = infos.extensions.size();
		deviceCreateInfos.ppEnabledExtensionNames = infos.extensions.data();
		deviceCreateInfos.enabledLayerCount = 0;
		deviceCreateInfos.ppEnabledLayerNames = nullptr;
		deviceCreateInfos.pEnabledFeatures = &features;
		deviceCreateInfos.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfos.pQueueCreateInfos = queueCreateInfos.data();

		if (vkCreateDevice(infos.device, &deviceCreateInfos, nullptr, &device) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create logical device");

		return device;
	}



	std::vector<VkDeviceQueueCreateInfo> Device::s_chooseQueues(const std::vector<QueueInfos> &queueInfos, QueueTypeMask wantedQueue) {
		std::vector<VkDeviceQueueCreateInfo> outputs {};

		
	}

} // namespace se::renderer::vulkan
