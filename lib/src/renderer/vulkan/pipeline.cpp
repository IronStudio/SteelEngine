#include "se/renderer/vulkan/pipeline.hpp"

#include <vulkan/vulkan.h>

#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/shader.hpp"
#include "se/renderer/vulkan/vertexBufferView.hpp"



namespace se::renderer::vulkan {
	Pipeline::Pipeline(const se::renderer::PipelineInfos &infos) :
		se::renderer::Pipeline(infos),
		m_pipeline {VK_NULL_HANDLE},
		m_pipelineLayout {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

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

		std::vector<VkDynamicState> dynamicStates {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfos {};
		dynamicStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfos.dynamicStateCount = dynamicStates.size();
		dynamicStateCreateInfos.pDynamicStates = dynamicStates.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfos {};
		inputAssemblyStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfos.primitiveRestartEnable = VK_FALSE;
		inputAssemblyStateCreateInfos.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewportStateCreateInfos {};
		viewportStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfos.scissorCount = 1;
		viewportStateCreateInfos.pScissors = nullptr;
		viewportStateCreateInfos.viewportCount = 1;
		viewportStateCreateInfos.pViewports = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfos {};
		rasterizationStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfos.depthClampEnable = VK_FALSE;
		rasterizationStateCreateInfos.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfos.lineWidth = 1.f;
		rasterizationStateCreateInfos.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCreateInfos.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCreateInfos.depthBiasEnable = VK_FALSE;
		rasterizationStateCreateInfos.depthBiasConstantFactor = 0.f;
		rasterizationStateCreateInfos.depthBiasClamp = 0.f;
		rasterizationStateCreateInfos.depthBiasSlopeFactor = 0.f;
		rasterizationStateCreateInfos.rasterizerDiscardEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfos {};
		multisampleStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfos.sampleShadingEnable = VK_FALSE;
		multisampleStateCreateInfos.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleStateCreateInfos.minSampleShading = 1.f;
		multisampleStateCreateInfos.pSampleMask = nullptr;
		multisampleStateCreateInfos.alphaToCoverageEnable = VK_FALSE;
		multisampleStateCreateInfos.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfos {};
		colorBlendStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateCreateInfos.attachmentCount = 1;
		colorBlendStateCreateInfos.pAttachments = &colorBlendAttachment;
		colorBlendStateCreateInfos.logicOpEnable = VK_FALSE;
		colorBlendStateCreateInfos.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateCreateInfos.blendConstants[0] = 0.f;
		colorBlendStateCreateInfos.blendConstants[1] = 0.f;
		colorBlendStateCreateInfos.blendConstants[2] = 0.f;
		colorBlendStateCreateInfos.blendConstants[3] = 0.f;

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfos {};
		pipelineLayoutCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfos.setLayoutCount = 0;
		pipelineLayoutCreateInfos.pSetLayouts = nullptr;
		pipelineLayoutCreateInfos.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfos.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfos, nullptr, &m_pipelineLayout) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create pipeline layout");

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos {};
		shaderStageCreateInfos.reserve(m_infos.shaders.size());
		for (const auto &shader : m_infos.shaders) {
			shaderStageCreateInfos.push_back(reinterpret_cast<const se::renderer::vulkan::Shader*> (shader)->getShaderStageCreateInfos());
		}

		const auto &swapchain {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getSwapchain()};
		VkFormat colorAttachmentFormat {swapchain->getFormat().format};
		VkPipelineRenderingCreateInfo pipelineRenderingCreateInfos {};
		pipelineRenderingCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingCreateInfos.colorAttachmentCount = 1;
		pipelineRenderingCreateInfos.pColorAttachmentFormats = &colorAttachmentFormat;
		pipelineRenderingCreateInfos.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
		pipelineRenderingCreateInfos.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
		pipelineRenderingCreateInfos.viewMask = 0;

		VkGraphicsPipelineCreateInfo pipelineCreateInfos {};
		pipelineCreateInfos.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfos.pNext = &pipelineRenderingCreateInfos;
		pipelineCreateInfos.stageCount = shaderStageCreateInfos.size();
		pipelineCreateInfos.pStages = shaderStageCreateInfos.data();
		pipelineCreateInfos.pVertexInputState = &vertexInputStateCreateInfos;
		pipelineCreateInfos.pInputAssemblyState = &inputAssemblyStateCreateInfos;
		pipelineCreateInfos.pViewportState = &viewportStateCreateInfos;
		pipelineCreateInfos.pRasterizationState = &rasterizationStateCreateInfos;
		pipelineCreateInfos.pMultisampleState = &multisampleStateCreateInfos;
		pipelineCreateInfos.pDepthStencilState = nullptr;
		pipelineCreateInfos.pColorBlendState = &colorBlendStateCreateInfos;
		pipelineCreateInfos.pDynamicState = &dynamicStateCreateInfos;
		pipelineCreateInfos.layout = m_pipelineLayout;
		pipelineCreateInfos.renderPass = VK_NULL_HANDLE;
		pipelineCreateInfos.subpass = 0;
		pipelineCreateInfos.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfos.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfos, nullptr, &m_pipeline) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create graphics pipeline");
	}


	Pipeline::~Pipeline() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_pipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(device, m_pipeline, nullptr);

		if (m_pipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
	}


} // namespace se::renderer::vulkan
