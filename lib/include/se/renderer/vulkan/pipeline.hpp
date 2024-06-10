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
			inline VkDescriptorSetLayout getDescriptorSetLayout() const noexcept {return m_descriptorSetLayout;}

		private:
			static VkDescriptorSetLayout s_createDescriptorSetLayout(
				VkDevice device,
				const std::vector<se::renderer::AttributeBufferView *> &attributeBufferViews
			);

			static VkPipelineLayout s_createPipelineLayout(
				VkDevice device,
				VkDescriptorSetLayout descriptorSetLayout
			);

			static VkPipeline s_createGraphicsPipeline(
				const se::renderer::PipelineInfos &infos,
				VkPipelineLayout pipelineLayout,
				VkDescriptorSetLayout descriptorSetLayout
			);

			static VkPipeline s_createComputePipeline(
				const se::renderer::PipelineInfos &infos,
				VkPipelineLayout pipelineLayout,
				VkDescriptorSetLayout descriptorSetLayout
			);

			VkPipeline m_pipeline;
			VkPipelineLayout m_pipelineLayout;
			VkDescriptorSetLayout m_descriptorSetLayout;
	};

} // namespace se::renderer::vulkan
