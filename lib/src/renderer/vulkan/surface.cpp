#include "se/renderer/vulkan/surface.hpp"

#include <SDL2/SDL_vulkan.h>

#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	Surface::Surface(const se::renderer::vulkan::SurfaceInfos &infos) :
		m_infos {infos},
		m_surface {VK_NULL_HANDLE},
		m_surfaceCapabilities {},
		m_surfaceFormats {},
		m_presentModes {}
	{
		if (!SDL_Vulkan_CreateSurface(m_infos.window->getInternalObject(), m_infos.instance, &m_surface))
			throw se::exceptions::RuntimeError("Can't create vulkan SDL2 surface : " + std::string(SDL_GetError()));

		SE_LOGGER.log({se::LogSeverity::eVerbose}, "Created a SDL2 VulkanKHR surface : {}", (void*)m_surface);
	}


	Surface::~Surface() {
		if (m_surface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(m_infos.instance, m_surface, nullptr);
	}


	void Surface::queryInformations(VkPhysicalDevice physicalDevice) {
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &m_surfaceCapabilities) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get physical device surface capabilities");

		se::Uint32 count {};
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &count, nullptr))
			throw se::exceptions::RuntimeError("Can't get physiccal device surface formats count");

		m_surfaceFormats.resize(count);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &count, m_surfaceFormats.data()))
			throw se::exceptions::RuntimeError("Can't get physiccal device surface formats");


		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &count, nullptr))
			throw se::exceptions::RuntimeError("Can't get physiccal device surface present modes count");

		m_surfaceFormats.resize(count);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &count, m_presentModes.data()))
			throw se::exceptions::RuntimeError("Can't get physiccal device surface present modes");
	}


} // namespace se::renderer::vulkan
