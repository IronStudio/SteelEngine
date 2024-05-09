#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/renderer/context.hpp"
#include "se/renderer/vulkan/device.hpp"



namespace se::renderer::vulkan {
	class Context : public se::renderer::Context {
		public:
			Context(const se::renderer::ContextInfos &infos);
			~Context() override;

			inline VkInstance getInstance() const noexcept {return m_instance;}

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

			VkInstance m_instance;
			se::renderer::vulkan::Device *m_device;
			#ifndef NDEBUG
				VkDebugUtilsMessengerEXT m_debugMessenger;
			#endif
	};

} // namespace se::renderer::vulkan
