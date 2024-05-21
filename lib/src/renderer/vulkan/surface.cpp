#include "se/renderer/vulkan/surface.hpp"

#include <SDL2/SDL_vulkan.h>

#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	Surface::Surface(const se::renderer::vulkan::SurfaceInfos &infos) :
		m_infos {infos},
		m_surface {VK_NULL_HANDLE}
	{
		if (!SDL_Vulkan_CreateSurface(m_infos.window->getInternalObject(), m_infos.instance, &m_surface))
			throw se::exceptions::RuntimeError("Can't create vulkan SDL2 surface : " + std::string(SDL_GetError()));

		SE_LOGGER.log({se::LogSeverity::eVerbose}, "Created a SDL2 VulkanKHR surface : {}", (void*)m_surface);
	}


	Surface::~Surface() {
		if (m_surface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(m_infos.instance, m_surface, nullptr);
	}


} // namespace se::renderer::vulkan
