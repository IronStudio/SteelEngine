#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/renderPass.hpp"



namespace se::renderer::vulkan {
	class SE_CORE RenderPass final : public se::renderer::RenderPass {
		public:
			RenderPass(const se::renderer::RenderPassInfos &infos);
			~RenderPass() override;

			inline VkRenderPass getRenderPass() const noexcept {return m_renderPass;}


		private:
			VkRenderPass m_renderPass;
	};
	
} // namespace se::renderer::vulkan
