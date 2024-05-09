#include "se/renderer/vulkan/context.hpp"

#include <cstring>
#include <iostream>
#include <vector>

#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	Context::Context(const se::renderer::ContextInfos &infos) : se::renderer::Context(infos),
		m_instance{VK_NULL_HANDLE},
		m_device {nullptr}
	#ifndef NDEBUG
		, m_debugMessenger{VK_NULL_HANDLE}
	#endif
	{
		std::vector<const char *> layers{
		#ifndef NDEBUG
			"VK_LAYER_KHRONOS_validation"
		#endif
		};
		std::vector<const char *> extensions{
		#ifndef NDEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
		#endif
		};

		layers = s_checkLayers(layers);
		extensions = s_checkExtensions(extensions);

		VkApplicationInfo applicationInfos{};
		applicationInfos.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfos.apiVersion = VK_API_VERSION_1_3;
		applicationInfos.pApplicationName = infos.applicationName.c_str();
		applicationInfos.applicationVersion = VK_MAKE_API_VERSION(
			0, infos.applicationVersion.major, infos.applicationVersion.minor, infos.applicationVersion.patch);
		applicationInfos.pEngineName = "SteelEngine";
		applicationInfos.engineVersion = VK_MAKE_API_VERSION(0, SE_VERSION_MAJOR, SE_VERSION_MINOR, SE_VERSION_PATCH);

		VkInstanceCreateInfo instanceCreateInfos{};
		instanceCreateInfos.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfos.enabledExtensionCount = (se::Uint32)extensions.size();
		instanceCreateInfos.ppEnabledExtensionNames = extensions.data();
		instanceCreateInfos.enabledLayerCount = (se::Uint32)layers.size();
		instanceCreateInfos.ppEnabledLayerNames = layers.data();
		instanceCreateInfos.pApplicationInfo = &applicationInfos;

		if (vkCreateInstance(&instanceCreateInfos, nullptr, &m_instance) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create instance");

		SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo);
		SE_LOGGER << "Created vulkan instance with the following parameters :\n";
		SE_LOGGER << "\tVulkan API version  : 1.3\n";
		SE_LOGGER << "\tApplication name    : " << m_infos.applicationName << "\n";
		SE_LOGGER << "\tApplication version : " << m_infos.applicationVersion << "\n";
		SE_LOGGER << "\tSteelEngine version : " << se::utils::Version(SE_VERSION_MAJOR, SE_VERSION_MINOR, SE_VERSION_PATCH) << "\n";
		SE_LOGGER << "\tLayers (" << layers.size() << ") :\n";
		for (const auto &layer : layers)
			SE_LOGGER << "\t\t- " << layer << "\n";
		SE_LOGGER << "\tExtensions (" << extensions.size() << ") :\n";
		for (const auto &extension : extensions)
			SE_LOGGER << "\t\t- " << extension << "\n";

		SE_LOGGER << se::endLog;

		#ifndef NDEBUG
			m_debugMessenger = s_createDebugMessenger(m_instance);
		#endif

		se::renderer::vulkan::DeviceInfos deviceInfos {};
		deviceInfos.instance = m_instance;
		deviceInfos.gpuType = m_infos.preferredGPU;
		m_device = new se::renderer::vulkan::Device(deviceInfos);
	}



	Context::~Context() {
		if (m_device != nullptr)
			delete m_device;

	#ifndef NDEBUG
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");

		if (m_debugMessenger != VK_NULL_HANDLE)
			vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
	#endif

		if (m_instance != VK_NULL_HANDLE)
			vkDestroyInstance(m_instance, nullptr);
	}



	VKAPI_ATTR VkBool32 VKAPI_CALL Context::s_debugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
		void *userData
	) {
		static const std::map<VkDebugUtilsMessageSeverityFlagBitsEXT, se::LogSeverity> severityMap {
			{VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, se::LogSeverity::eVerbose},
			{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, se::LogSeverity::eWarning},
			{VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, se::LogSeverity::eError}
		};

		if (severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			return VK_FALSE;

		std::string typeString{};

		switch (type)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			typeString = "general";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			typeString = "validation";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			typeString = "performance";
			break;
		default:
			typeString = "unknown";
		}

		SE_LOGGER.log(se::LogInfos(severityMap.find(severity)->second), "Vulkan log [{}] > {}", typeString, callbackData->pMessage);
		return VK_FALSE;
	}



	VkDebugUtilsMessengerEXT Context::s_createDebugMessenger(VkInstance instance) {
		VkDebugUtilsMessengerEXT debugMessenger{};

		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfos{};
		debugMessengerCreateInfos.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfos.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
		debugMessengerCreateInfos.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerCreateInfos.pfnUserCallback = s_debugMessageCallback;
		debugMessengerCreateInfos.pUserData = nullptr;

		if (vkCreateDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfos, nullptr, &debugMessenger) != VK_SUCCESS) {
			SE_LOGGER.log(se::LogInfos(se::LogSeverity::eError), "Can't create debug utils messenger");
			return VK_NULL_HANDLE;
		}

		return debugMessenger;
	}



	std::vector<const char *> Context::s_checkLayers(const std::vector<const char *> &layers) {
		if (layers.empty())
			return {};

		se::Uint32 count{};
		if (vkEnumerateInstanceLayerProperties(&count, nullptr) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get instance layer count");

		std::vector<VkLayerProperties> availableLayers{};
		availableLayers.resize(count);
		if (vkEnumerateInstanceLayerProperties(&count, availableLayers.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get instance layers");

		std::vector<const char *> finalLayers{};
		finalLayers.reserve(layers.size());

		for (const auto &layer : layers) {
			for (const auto &available : availableLayers) {
				if (strcmp(available.layerName, layer) != 0)
					continue;
				finalLayers.push_back(layer);
				break;
			}
		}

		return finalLayers;
	}



	std::vector<const char *> Context::s_checkExtensions(const std::vector<const char *> &extensions) {
		if (extensions.empty())
			return {};

		se::Uint32 count{};
		if (vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get instance extension count");

		std::vector<VkExtensionProperties> availableExtensions{};
		availableExtensions.resize(count);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &count, availableExtensions.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get instance extensions");

		std::vector<const char *> finalExtensions{};
		finalExtensions.reserve(extensions.size());

		for (const auto &extension : extensions) {
			for (const auto &available : availableExtensions) {
				if (strcmp(available.extensionName, extension) != 0)
					continue;
				finalExtensions.push_back(extension);
				break;
			}
		}

		return finalExtensions;
	}

} // namespace se::renderer::vulkan
