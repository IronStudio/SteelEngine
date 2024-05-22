#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/window/window.hpp"



namespace se::renderer::vulkan {
	struct SurfaceInfos {
		VkInstance instance;
		se::window::Window *window;
	};

	class Surface {
		public:
			Surface(const se::renderer::vulkan::SurfaceInfos &infos);
			~Surface();

			void queryInformations(VkPhysicalDevice physicalDevice);

			inline VkSurfaceKHR getSurface() const noexcept {return m_surface;}
			inline const std::vector<VkSurfaceFormatKHR> &getSurfaceFormats() const noexcept {return m_surfaceFormats;}
			inline const std::vector<VkPresentModeKHR> &getPresentModes() const noexcept {return m_presentModes;}
			inline const VkSurfaceCapabilitiesKHR &getSurfaceCapabilites() const noexcept {return m_surfaceCapabilities;}

		private:
			se::renderer::vulkan::SurfaceInfos m_infos;
			VkSurfaceKHR m_surface;
			VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
			std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
			std::vector<VkPresentModeKHR> m_presentModes;
	};

} // namespace se::renderer::vulkan
