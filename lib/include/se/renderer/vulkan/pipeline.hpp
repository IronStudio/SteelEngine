#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/pipeline.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Pipeline final : public se::renderer::Pipeline {
		public:
			Pipeline(const se::renderer::PipelineInfos &infos);
			~Pipeline() override;

			inline VkPipeline getPipeline() const noexcept {return m_pipeline;}
			inline VkPipelineLayout getPipelineLayout() const noexcept {return m_pipelineLayout;}

		private:
			VkPipeline m_pipeline;
			VkPipelineLayout m_pipelineLayout;
	};

} // namespace se::renderer::vulkan
