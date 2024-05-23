#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/vulkan/device.hpp"
#include "se/renderer/vulkan/surface.hpp"
#include "se/renderer/vulkan/swapchain.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Context : public se::renderer::Context {
		public:
			Context(const se::renderer::ContextInfos &infos);
			~Context() override;

			void handleWindowSizeChanged() override;

			inline VkInstance getInstance() const noexcept {return s_instance;}
			inline se::renderer::vulkan::Device *getDevice() const noexcept {return m_device;}

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL s_debugMessageCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT severity,
				VkDebugUtilsMessageTypeFlagsEXT type,
				const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
				void *userData
			);

			static VkDebugUtilsMessengerEXT s_createDebugMessenger(VkInstance instance);
			static std::vector<const char*> s_checkLayers(const std::vector<const char*> &layers);
			static std::vector<const char*> s_checkExtensions(const std::vector<const char*> &extensions);

			static VkInstance s_instance;
			static se::Count s_instanceCount;
			se::renderer::vulkan::Surface *m_surface;
			se::renderer::vulkan::Device *m_device;
			se::renderer::vulkan::Swapchain *m_swapchain;
			#ifndef NDEBUG
				VkDebugUtilsMessengerEXT m_debugMessenger;
			#endif
	};

} // namespace se::renderer::vulkan
