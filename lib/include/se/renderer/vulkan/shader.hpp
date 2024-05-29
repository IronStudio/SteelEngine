#pragma once

#include <map>
#include <string>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/shader.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Shader final : public se::renderer::Shader {
		public:
			Shader(const se::renderer::ShaderInfos &infos);
			~Shader() override;

			inline VkShaderModule getShaderModule() const noexcept {return m_shaderModule;}
			inline const VkPipelineShaderStageCreateInfo &getShaderStageCreateInfos() const noexcept {return m_shaderStageCreateInfos;}

		private:
			static VkShaderModule s_loadShaderModule(se::renderer::Context *context, const std::string &file);
			static void s_unloadShaderModule(se::renderer::Context *context, VkShaderModule shaderModule);

			static std::map<std::string, VkShaderModule> s_moduleMap;
			static std::map<VkShaderModule, se::Count> s_instanceCountMap;

			VkShaderModule m_shaderModule;
			VkPipelineShaderStageCreateInfo m_shaderStageCreateInfos;
	};

} // namespace se::renderer::vulkan
