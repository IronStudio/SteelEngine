#include "se/renderer/vulkan/device.hpp"

#include <bitset>
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

	VkQueueFlags queueTypeMaskSeToVk(se::renderer::vulkan::QueueTypeMask queue) {
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
		m_physicalDevice {VK_NULL_HANDLE},
		m_queues {}
	{
		ScoreCriterias scoreCriterias {};
		scoreCriterias.gpuType = m_infos.gpuType;
		scoreCriterias.extensions = m_infos.extensions;
		scoreCriterias.queueTypeMask = m_infos.queueTypeMask;
		scoreCriterias.surface = m_infos.surface;
		m_physicalDevice = s_chooseDevice(m_infos.instance, scoreCriterias);

		DeviceCreateInfos deviceCreateInfos {};
		deviceCreateInfos.device = m_physicalDevice;
		deviceCreateInfos.extensions = m_infos.extensions;
		deviceCreateInfos.queueTypeMask = m_infos.queueTypeMask;
		deviceCreateInfos.surface = m_infos.surface;
		deviceCreateInfos.features = m_infos.requiredFeatures;

		m_device = s_createDevice(deviceCreateInfos, m_queues);

		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Queues :\n";
		for (const auto &queue : m_queues)
			SE_LOGGER << "\t" << (se::Uint64)queue.first << " : " << queue.second.size() << "\n";
		SE_LOGGER << se::endLog;

		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Device extensions :\n";
		for (const auto &extension : m_infos.extensions)
			SE_LOGGER << "\t" << extension << "\n";
		SE_LOGGER << se::endLog;
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
		se::Count index {0};
		for (const auto &queue : queueFamilies) {
			foundQueueTypes |= queueTypeMaskVkToSe(queue.queueFlags);
			VkBool32 support {VK_FALSE};
			if (vkGetPhysicalDeviceSurfaceSupportKHR(device, index, criterias.surface, &support) != VK_SUCCESS)
				SE_LOGGER.log({se::LogSeverity::eWarning}, "Can't check surface support of queue family {}", index);
			if (support == VK_TRUE)
				foundQueueTypes |= se::renderer::vulkan::QueueType::ePresent;
			++index;
		}

		if (criterias.queueTypeMask != (criterias.queueTypeMask & foundQueueTypes))
			return -1;

		return score;
	}


	VkDevice Device::s_createDevice(const DeviceCreateInfos &infos, std::map<QueueType, std::vector<VkQueue>> &queues) {
		VkDevice device {};

		se::Uint32 queueCount {};
		vkGetPhysicalDeviceQueueFamilyProperties(infos.device, &queueCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies {};
		queueFamilies.resize(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(infos.device, &queueCount, queueFamilies.data());


		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo);
		SE_LOGGER << "Available queue families :\n";
		for (se::Count i {0}; i < queueFamilies.size(); ++i) {
			VkBool32 presentSupported {};
			if (vkGetPhysicalDeviceSurfaceSupportKHR(infos.device, i, infos.surface, &presentSupported) != VK_SUCCESS)
				presentSupported = VK_FALSE;

			se::renderer::vulkan::QueueTypeMask queueType {queueTypeMaskVkToSe(queueFamilies[i].queueFlags)};
			if(presentSupported)
				queueType |= se::renderer::vulkan::QueueType::ePresent;

			SE_LOGGER << "\t- Queue index " << i << "\n";
			SE_LOGGER << "\t\t- Type  : " << std::bitset<8>(queueType.content) << "\n";
			SE_LOGGER << "\t\t- Count : " << queueFamilies[i].queueCount << "\n";
		}

		SE_LOGGER << se::endLog;


		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {};
		std::vector<std::vector<float>> priorities {};
		std::vector<QueueInfos> queueInfosForSelection {};
		queueCreateInfos.reserve(queueCount);
		priorities.reserve(queueCount);
		queueInfosForSelection.reserve(queueCount);

		for (se::Count i {0}; i < queueFamilies.size(); ++i) {
			VkDeviceQueueCreateInfo queueInfos {};
			queueInfos.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfos.queueCount = queueFamilies[i].queueCount;
			queueInfos.queueFamilyIndex = i;

			priorities.push_back({});
			priorities.rbegin()->resize(queueInfos.queueCount);
			for (se::Count j {0}; j < queueInfos.queueCount; ++j)
				priorities.rbegin()->operator[](j) = 1.f;

			queueInfos.pQueuePriorities = priorities.rbegin()->data();
			queueCreateInfos.push_back(queueInfos);

			se::renderer::vulkan::QueueTypeMask queueFlags {queueTypeMaskVkToSe(queueFamilies[i].queueFlags)};

			VkBool32 presentSupported {};
			if (vkGetPhysicalDeviceSurfaceSupportKHR(infos.device, i, infos.surface, &presentSupported) != VK_SUCCESS)
				presentSupported = VK_FALSE;
			if (presentSupported)
				queueFlags |= se::renderer::vulkan::QueueType::ePresent;

			QueueInfos queueInfosSelection {};
			queueInfosSelection.count = queueInfos.queueCount;
			queueInfosSelection.type = queueFlags;
			queueInfosForSelection.push_back(queueInfosSelection);
		}

		VkDeviceCreateInfo deviceCreateInfos {};
		deviceCreateInfos.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfos.enabledExtensionCount = infos.extensions.size();
		deviceCreateInfos.ppEnabledExtensionNames = infos.extensions.data();
		deviceCreateInfos.enabledLayerCount = 0;
		deviceCreateInfos.ppEnabledLayerNames = nullptr;
		deviceCreateInfos.pEnabledFeatures = nullptr;
		deviceCreateInfos.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfos.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfos.pNext = &infos.features;

		if (vkCreateDevice(infos.device, &deviceCreateInfos, nullptr, &device) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create logical device");

		queues = s_getQueues(device, queueInfosForSelection);
		return device;
	}



	std::map<QueueType, std::vector<VkQueue>> Device::s_getQueues(VkDevice device, const std::vector<QueueInfos> &queueInfos) {
		std::map<QueueType, std::vector<VkQueue>> output {};

		for (se::Count i {0}; i < queueInfos.size(); ++i) {
			std::vector<VkQueue> queues {};
			queues.resize(queueInfos[i].count);
			for (se::Count j {0}; j < queueInfos[i].count; ++j) {
				VkQueue queue {};
				vkGetDeviceQueue(device, i, j, &queue);
				queues[j] = queue;
			}

			for (se::Count j {0}; j < SE_QUEUE_TYPE_COUNT; ++j) {
				if (!(queueInfos[i].type & static_cast<QueueType> (1 << j)))
					continue;

				output[static_cast<QueueType> (1 << j)].insert(output[static_cast<QueueType> (1 << j)].begin(), queues.begin(), queues.end());
			}
		}

		return output;
	}

} // namespace se::renderer::vulkan
