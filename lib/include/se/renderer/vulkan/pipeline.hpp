#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/pipeline.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Pipeline final : public se::renderer::Pipeline {
		public:
			Pipeline(const se::renderer::PipelineInfos &infos);
			~Pipeline() override;

		private:
			VkPipeline m_pipeline;
	};

} // namespace se::renderer::vulkan
