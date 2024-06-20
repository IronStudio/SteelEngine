#include "se/renderer/vulkan/pipeline.hpp"

#include <vulkan/vulkan.h>

#include "se/assertion.hpp"
#include "se/logger.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/format.hpp"
#include "se/renderer/vulkan/shader.hpp"
#include "se/renderer/vulkan/attributeBufferView.hpp"
#include "se/renderer/vulkan/vertexBufferView.hpp"



namespace se::renderer::vulkan {
	Pipeline::Pipeline(const se::renderer::PipelineInfos &infos) :
		se::renderer::Pipeline(infos),
		m_pipeline {VK_NULL_HANDLE},
		m_pipelineLayout {VK_NULL_HANDLE},
		m_descriptorSetLayout {VK_NULL_HANDLE},
		m_descriptorPool {VK_NULL_HANDLE},
		m_descriptorSet {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (!m_infos.attributeBufferView.empty())
			m_descriptorSetLayout = s_createDescriptorSetLayout(
				device,
				m_infos.attributeBufferView,
				m_infos.type,
				m_descriptorPool,
				m_descriptorSet
			);

		m_pipelineLayout = s_createPipelineLayout(device, m_descriptorSetLayout);

		if (m_infos.type == se::renderer::PipelineType::eRasterization)
			m_pipeline = s_createGraphicsPipeline(m_infos, m_pipelineLayout, m_descriptorSetLayout);
		else if (m_infos.type == se::renderer::PipelineType::eCompute)
			m_pipeline = s_createComputePipeline(m_infos, m_pipelineLayout, m_descriptorSetLayout);
	}


	Pipeline::~Pipeline() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_descriptorSetLayout != VK_NULL_HANDLE)
			vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);

		if (m_pipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(device, m_pipeline, nullptr);

		if (m_pipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
	}


	void Pipeline::updateBuffer(VkCommandBuffer commandBuffer) {
		static const std::map<se::renderer::PipelineType, VkDescriptorType> pipelineTypeToDescriptor {
			{se::renderer::PipelineType::eRasterization, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER},
			{se::renderer::PipelineType::eCompute,       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER}
		};

		auto &device {*reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()};

		std::vector<VkDescriptorBufferInfo> descriptorBuffers {};

		VkDescriptorBufferInfo descriptorBufferInfos {};
		descriptorBufferInfos.buffer = uniformBuffer.getBuffer();
		descriptorBufferInfos.offset = uniformBufferView.getInfos().offset;
		descriptorBufferInfos.range = uniformBufferView.getTotalSize();

		VkWriteDescriptorSet writeDescriptorSet {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = m_descriptorSet;
		writeDescriptorSet.dstBinding = uniformBufferView.getInfos().binding;
		writeDescriptorSet.dstArrayElement = 0;
		writeDescriptorSet.descriptorType = pipelineTypeToDescriptor.find(m_infos.type)->second;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.pBufferInfo = &descriptorBufferInfos;
		writeDescriptorSet.pImageInfo = nullptr;
		writeDescriptorSet.pTexelBufferView = nullptr;
		vkUpdateDescriptorSets(device.getDevice(), 1, &writeDescriptorSet, 0, nullptr);
	}


	VkDescriptorSetLayout Pipeline::s_createDescriptorSetLayout(
		VkDevice device,
		const std::vector<se::renderer::AttributeBufferView *> &attributeBufferViews,
		se::renderer::PipelineType pipelineType,
		VkDescriptorPool &descriptorPool,
		VkDescriptorSet &descriptorSet
	) {
		static const std::map<se::renderer::PipelineType, VkDescriptorType> pipelineTypeToDescriptor {
			{se::renderer::PipelineType::eRasterization, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER},
			{se::renderer::PipelineType::eCompute,       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER}
		};

		std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings {};
		descriptorSetLayoutBindings.reserve(attributeBufferViews.size());
		for (const auto &abv : attributeBufferViews) {
			auto attributeBufferView {reinterpret_cast<se::renderer::vulkan::AttributeBufferView*> (abv)};
			descriptorSetLayoutBindings.push_back(attributeBufferView->getLayoutBinding());
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfos {};
		descriptorSetLayoutCreateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfos.bindingCount = descriptorSetLayoutBindings.size();
		descriptorSetLayoutCreateInfos.pBindings = descriptorSetLayoutBindings.data();

		VkDescriptorSetLayout descriptorSetLayout {VK_NULL_HANDLE};
		if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfos, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create descriptor set layout");


		VkDescriptorPoolSize descriptorPoolSize {};
		descriptorPoolSize.type = pipelineTypeToDescriptor.find(pipelineType)->second;
		descriptorPoolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfos {};
		descriptorPoolCreateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfos.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfos.maxSets = 1;
		descriptorPoolCreateInfos.poolSizeCount = 1;
		descriptorPoolCreateInfos.pPoolSizes = &descriptorPoolSize;
		if (vkCreateDescriptorPool(device, &descriptorPoolCreateInfos, nullptr, &descriptorPool) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create descriptor pool");

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfos {};
		descriptorSetAllocateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfos.descriptorPool = descriptorPool;
		descriptorSetAllocateInfos.descriptorSetCount = 1;
		descriptorSetAllocateInfos.pSetLayouts = &descriptorSetLayout;
		if (vkAllocateDescriptorSets(device, &descriptorSetAllocateInfos, &descriptorSet) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create descriptor set");

		return descriptorSetLayout;
	}


	VkPipelineLayout Pipeline::s_createPipelineLayout(
		VkDevice device,
		VkDescriptorSetLayout descriptorSetLayout
	) {
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfos {};
		pipelineLayoutCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfos.setLayoutCount = 0;
		pipelineLayoutCreateInfos.pSetLayouts = nullptr;
		pipelineLayoutCreateInfos.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfos.pPushConstantRanges = nullptr;

		if (descriptorSetLayout != VK_NULL_HANDLE) {
			pipelineLayoutCreateInfos.setLayoutCount = 1;
			pipelineLayoutCreateInfos.pSetLayouts = &descriptorSetLayout;
		}

		VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
		if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfos, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create pipeline layout");
		return pipelineLayout;
	}


	VkPipeline Pipeline::s_createGraphicsPipeline(
		const se::renderer::PipelineInfos &infos,
		VkPipelineLayout pipelineLayout,
		VkDescriptorSetLayout descriptorSetLayout
	) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfos {};
		vertexInputStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		if (infos.vertexBufferView != nullptr) {
			auto vertexBufferView {reinterpret_cast<se::renderer::vulkan::VertexBufferView*> (infos.vertexBufferView)};
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
		inputAssemblyStateCreateInfos.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;//VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

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
		if (infos.blendMode == se::renderer::BlendMode::eSrcAlpha) {
			colorBlendAttachment.blendEnable = VK_TRUE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		}

		else {
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		}

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

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos {};
		shaderStageCreateInfos.reserve(infos.shaders.size());
		for (const auto &shader : infos.shaders) {
			shaderStageCreateInfos.push_back(reinterpret_cast<const se::renderer::vulkan::Shader*> (shader)->getShaderStageCreateInfos());
		}

		std::vector<VkFormat> colorAttachmentFormats {};
		colorAttachmentFormats.reserve(infos.colorAttachmentFormats.size());
		for (const auto &format : infos.colorAttachmentFormats)
			colorAttachmentFormats.push_back(se::renderer::vulkan::formatSeToVk(format));

		const auto &swapchain {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getSwapchain()};
		VkFormat colorAttachmentFormat {swapchain->getFormat().format};
		VkPipelineRenderingCreateInfo pipelineRenderingCreateInfos {};
		pipelineRenderingCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingCreateInfos.colorAttachmentCount = colorAttachmentFormats.size();
		pipelineRenderingCreateInfos.pColorAttachmentFormats = colorAttachmentFormats.data();
		pipelineRenderingCreateInfos.depthAttachmentFormat = se::renderer::vulkan::formatSeToVk(infos.depthAttachmentFormat);
		pipelineRenderingCreateInfos.stencilAttachmentFormat = se::renderer::vulkan::formatSeToVk(infos.stencilAttachmentFormat);
		pipelineRenderingCreateInfos.viewMask = 0;

		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfos {};
		pipelineDepthStencilStateCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		if (infos.depthAttachmentFormat == se::renderer::Format::eNone) {
			pipelineDepthStencilStateCreateInfos.depthTestEnable = VK_FALSE;
			pipelineDepthStencilStateCreateInfos.depthWriteEnable = VK_FALSE;
			pipelineDepthStencilStateCreateInfos.depthBoundsTestEnable = VK_FALSE;
		}

		else {
			pipelineDepthStencilStateCreateInfos.depthTestEnable = VK_TRUE;
			pipelineDepthStencilStateCreateInfos.depthWriteEnable = VK_TRUE;
			pipelineDepthStencilStateCreateInfos.depthBoundsTestEnable = VK_TRUE;
		}

		pipelineDepthStencilStateCreateInfos.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		pipelineDepthStencilStateCreateInfos.stencilTestEnable = VK_FALSE;
		pipelineDepthStencilStateCreateInfos.front = {};
		pipelineDepthStencilStateCreateInfos.back = {};
		pipelineDepthStencilStateCreateInfos.minDepthBounds = 0.f;
		pipelineDepthStencilStateCreateInfos.maxDepthBounds = 1.f;

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
		pipelineCreateInfos.pDepthStencilState = &pipelineDepthStencilStateCreateInfos;
		pipelineCreateInfos.pColorBlendState = &colorBlendStateCreateInfos;
		pipelineCreateInfos.pDynamicState = &dynamicStateCreateInfos;
		pipelineCreateInfos.layout = pipelineLayout;
		pipelineCreateInfos.renderPass = VK_NULL_HANDLE;
		pipelineCreateInfos.subpass = 0;
		pipelineCreateInfos.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfos.basePipelineIndex = -1;

		VkPipeline pipeline {VK_NULL_HANDLE};
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfos, nullptr, &pipeline) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create graphics pipeline");
		return pipeline;
	}


	VkPipeline Pipeline::s_createComputePipeline(
		const se::renderer::PipelineInfos &infos,
		VkPipelineLayout pipelineLayout,
		VkDescriptorSetLayout descriptorSetLayout
	) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};

		SE_ASSERT(infos.shaders.size() == 1, "Compute pipeline must have exactly one shader stage");

		VkPipelineShaderStageCreateInfo shaderStageCreateInfos {};
		shaderStageCreateInfos = reinterpret_cast<const se::renderer::vulkan::Shader*> (infos.shaders[0])->getShaderStageCreateInfos();

		VkComputePipelineCreateInfo pipelineCreateInfos {};
		pipelineCreateInfos.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfos.basePipelineIndex = -1;
		pipelineCreateInfos.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfos.layout = pipelineLayout;
		pipelineCreateInfos.stage = shaderStageCreateInfos;

		VkPipeline pipeline {VK_NULL_HANDLE};
		if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfos, nullptr, &pipeline) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create compute pipeline");
		return pipeline;
	}


} // namespace se::renderer::vulkan
