#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/pipeline.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Pipeline final : public se::renderer::Pipeline {
		public:
			Pipeline(const se::renderer::PipelineInfos &infos);
			~Pipeline() override;

			inline const VkPipeline &getPipeline() const noexcept {return m_pipeline;}
			inline const VkPipelineLayout &getPipelineLayout() const noexcept {return m_pipelineLayout;}
			inline const VkDescriptorSetLayout &getDescriptorSetLayout() const noexcept {return m_descriptorSetLayout;}
			inline const VkDescriptorSet &getDescriptorSet() const noexcept {return m_descriptorSet;}

			void updateBuffer();

		private:
			static VkDescriptorSetLayout s_createDescriptorSetLayout(
				VkDevice device,
				const std::vector<se::renderer::AttributeBufferView *> &attributeBufferViews,
				const std::vector<se::renderer::RangeBufferView *> &rangeBufferViews,
				VkDescriptorPool &descriptorPool,
				VkDescriptorSet &descriptorSet,
				se::renderer::PipelineType pipelineType
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
			VkDescriptorPool m_descriptorPool;
			VkDescriptorSet m_descriptorSet;
	};

} // namespace se::renderer::vulkan
