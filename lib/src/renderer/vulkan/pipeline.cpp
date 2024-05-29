#include "se/renderer/vulkan/pipeline.hpp"

#include <vulkan/vulkan.h>

#include "se/renderer/vulkan/vertexBufferView.hpp"



namespace se::renderer::vulkan {
	Pipeline::Pipeline(const se::renderer::PipelineInfos &infos) :
		se::renderer::Pipeline(infos),
		m_pipeline {VK_NULL_HANDLE}
	{
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfos {};
		vertexInputStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		if (m_infos.vertexBufferView != nullptr) {
			auto vertexBufferView {reinterpret_cast<se::renderer::vulkan::VertexBufferView*> (m_infos.vertexBufferView)};
			vertexInputStateCreateInfos.vertexAttributeDescriptionCount = vertexBufferView->getAttributeDescriptions().size();
			vertexInputStateCreateInfos.pVertexAttributeDescriptions = vertexBufferView->getAttributeDescriptions().data();
			vertexInputStateCreateInfos.vertexBindingDescriptionCount = vertexBufferView->getBindingDescriptions().size();
			vertexInputStateCreateInfos.pVertexBindingDescriptions = vertexBufferView->getBindingDescriptions().data();
		}

		else {
			vertexInputStateCreateInfos.vertexAttributeDescriptionCount = 0;
			vertexInputStateCreateInfos.pVertexAttributeDescriptions = nullptr;
			vertexInputStateCreateInfos.vertexBindingDescriptionCount = 0;
			vertexInputStateCreateInfos.pVertexBindingDescriptions = nullptr;
		}

		
	}


	Pipeline::~Pipeline() {

	}


} // namespace se::renderer::vulkan
