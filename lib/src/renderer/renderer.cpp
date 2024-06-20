#include "se/renderer/renderer.hpp"

#include "se/logger.hpp"
#include "se/utils/converter.hpp"

#include "se/renderer/vulkan/commandBuffer.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/pipeline.hpp"
#include "se/renderer/vulkan/synchronisation.hpp"



namespace se::renderer {

	Renderer::Renderer(const se::renderer::RendererInfos &infos) :
		m_infos {infos},
		m_context {},
		m_computeShader {},
		m_gpuOnlyAllocator {},
		m_stagingAllocator {},
		m_worldMapBufferView {},
		m_cameraBufferView {},
		m_hittedBlockBufferView {},
		m_worldMapBuffer {},
		m_cameraBuffer {},
		m_hittedBlocksBuffer {},
		m_cameraStagingBuffer {},
		m_computePipeline {}
	{
		se::ResourceManager::RendererContext contextInfos {};
		contextInfos.api = m_infos.api;
		contextInfos.applicationName = m_infos.application->getInfos().name;
		contextInfos.applicationVersion = m_infos.application->getInfos().version;
		contextInfos.linkedWindow = m_infos.window;
		contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
		m_context = std::move(se::ResourceManager::load(contextInfos));

		se::ResourceManager::RendererShader computeShaderInfos {};
		computeShaderInfos.context = m_context.res;
		computeShaderInfos.file = "shaders/test.comp";
		computeShaderInfos.entryPoint = "main";
		computeShaderInfos.type = se::renderer::ShaderType::eCompute;
		m_computeShader = std::move(se::ResourceManager::load(computeShaderInfos));



		se::ResourceManager::RendererVramAllocator allocatorInfos {};
		allocatorInfos.context = m_context.res;
		allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eDynamic;
		allocatorInfos.usageNature = se::renderer::VramUsageNature::eGpuToGpu;
		allocatorInfos.chunkSize = 256_MiB;
		m_gpuOnlyAllocator = std::move(se::ResourceManager::load(allocatorInfos));

		allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eStatic;
		allocatorInfos.usageNature = se::renderer::VramUsageNature::eCpuToGpu;
		allocatorInfos.chunkSize = 64_MiB;
		m_stagingAllocator = std::move(se::ResourceManager::load(allocatorInfos));



		se::ResourceManager::RendererAttributeBufferView attributeBufferViewInfos {};
		attributeBufferViewInfos.context = m_context.res;
		attributeBufferViewInfos.shaderTypes = se::renderer::ShaderType::eCompute;
		attributeBufferViewInfos.usage = se::renderer::AttributeBufferViewUsage::eStorage;
		attributeBufferViewInfos.offset = 0;

		attributeBufferViewInfos.binding = 0;
		attributeBufferViewInfos.attributes = {};
		m_worldMapBufferView = std::move(se::ResourceManager::load(attributeBufferViewInfos));

		attributeBufferViewInfos.binding = 1;
		attributeBufferViewInfos.attributes = {
			{"cameraPosition", se::renderer::AttributeType::eVec3},
			{"fov", se::renderer::AttributeType::eFloat32},
			{"cameraOrientation", se::renderer::AttributeType::eVec2},
			{"screenSize", se::renderer::AttributeType::eVec2i}
		};
		m_cameraBufferView = std::move(se::ResourceManager::load(attributeBufferViewInfos));

		attributeBufferViewInfos.binding = 2;
		attributeBufferViewInfos.attributes = {};
		m_hittedBlockBufferView = std::move(se::ResourceManager::load(attributeBufferViewInfos));



		se::ResourceManager::RendererBuffer bufferInfos {};
		bufferInfos.context = m_context.res;
		bufferInfos.allocator = m_gpuOnlyAllocator.res;
		bufferInfos.usage = se::renderer::BufferUsage::eStorage | se::renderer::BufferUsage::eTransferDst;

		bufferInfos.size = m_cameraBufferView.res->getTotalSize();
		m_worldMapBuffer = std::move(se::ResourceManager::load(bufferInfos));

		bufferInfos.size = m_cameraBufferView.res->getTotalSize();
		m_cameraBuffer = std::move(se::ResourceManager::load(bufferInfos));

		bufferInfos.size = sizeof(se::Float32) * 3 * m_infos.window->getInfos().size.x * m_infos.window->getInfos().size.y;
		m_hittedBlocksBuffer = std::move(se::ResourceManager::load(bufferInfos));

		bufferInfos.allocator = m_stagingAllocator.res;
		bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
		bufferInfos.size = m_cameraBufferView.res->getTotalSize();
		m_cameraStagingBuffer = std::move(se::ResourceManager::load(bufferInfos));


		se::Vec3f cameraPosition {0.f, 0.f, 0.f};
		auto fov {se::Float32(70_deg)};
		se::Vec2f cameraOrientation {0.f, 0.f};
		se::Vec2i screenSize {m_infos.window->getInfos().size};

		se::renderer::BufferWriteAttributeInfos bufferAttributeWriteInfos {};
		bufferAttributeWriteInfos.offset = 0;
		bufferAttributeWriteInfos.attributes = {
			{"cameraPosition", se::utils::vectorize(cameraPosition)},
			{"fov", se::utils::vectorize(fov)},
			{"cameraOrientation", se::utils::vectorize(cameraOrientation)},
			{"screenSize", se::utils::vectorize(screenSize)}
		};
		bufferAttributeWriteInfos.attributeBufferView = m_cameraBufferView.res;
		m_cameraStagingBuffer.res->write(bufferAttributeWriteInfos);


		se::ResourceManager::RendererBufferTransferor bufferTransferorInfos {};
		bufferTransferorInfos.context = m_context.res;
		se::Resource<se::renderer::BufferTransferor> bufferTransferor {std::move(se::ResourceManager::load(bufferTransferorInfos))};

		se::renderer::BufferTransferInfos transferInfos {};
		transferInfos.source = m_cameraStagingBuffer.res;
		transferInfos.destination = m_cameraBuffer.res;
		transferInfos.srcOffset = 0;
		transferInfos.dstOffset = 0;
		transferInfos.size = m_cameraBuffer.res->getInfos().size;
		bufferTransferor.res->transfer(transferInfos);


		se::ResourceManager::RendererComputePipeline computePipelineInfos {};
		computePipelineInfos.context = m_context.res;
		computePipelineInfos.shaders = {m_computeShader.res};
		computePipelineInfos.attributeBufferView = {m_worldMapBufferView.res, m_cameraBufferView.res, m_hittedBlockBufferView.res};
		m_computePipeline = std::move(se::ResourceManager::load(computePipelineInfos));


		bufferTransferor.res->sync();
	}


	Renderer::~Renderer() {
		//se::ResourceManager::unload(m_context);
	}


	void Renderer::render() {
		auto *context {reinterpret_cast<se::renderer::vulkan::Context*> (m_context.res)};

		se::renderer::vulkan::CommandBufferInfos commandBufferInfos {};
		commandBufferInfos.context = m_context.res;
		commandBufferInfos.queue = se::renderer::vulkan::QueueType::eCompute;
		se::renderer::vulkan::CommandBuffer commandBuffer {commandBufferInfos};

		se::renderer::vulkan::FenceInfos fenceInfos {};
		fenceInfos.context = context;
		fenceInfos.signaled = false;
		se::renderer::vulkan::Fence fence {fenceInfos};

		VkCommandBufferBeginInfo beginInfos {};
		beginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		(void)vkBeginCommandBuffer(commandBuffer.getCommandBuffer(), &beginInfos);


		vkCmdBindPipeline(
			commandBuffer.getCommandBuffer(),
			VK_PIPELINE_BIND_POINT_COMPUTE,
			reinterpret_cast<se::renderer::vulkan::Pipeline*> (m_computePipeline.res)->getPipeline()
		);
		
		vkCmdBindDescriptorSets(
			commandBuffer.getCommandBuffer(),
			VK_PIPELINE_BIND_POINT_COMPUTE,
			reinterpret_cast<se::renderer::vulkan::Pipeline*> (m_computePipeline.res)->getPipelineLayout(),
			0, 1, &reinterpret_cast<se::renderer::vulkan::Pipeline*> (m_computePipeline.res)->getDescriptorSet(),
			0, nullptr
		);

		vkCmdDispatch(commandBuffer.getCommandBuffer(), m_infos.window->getInfos().size.x / 256, m_infos.window->getInfos().size.y / 256, 1);


		(void)vkEndCommandBuffer(commandBuffer.getCommandBuffer());

		VkSubmitInfo submitInfos {};
		submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfos.commandBufferCount = 1;
		submitInfos.pCommandBuffers = &commandBuffer.getCommandBuffer();
		submitInfos.waitSemaphoreCount = 0;
		submitInfos.signalSemaphoreCount = 0;
		(void)vkQueueSubmit(context->getDevice()->getQueue(se::renderer::vulkan::QueueType::eCompute), 1, &submitInfos, fence.getFence());

		se::renderer::vulkan::Fence::sync(SE_NEVER_TIMEOUT, true, {&fence});
		se::renderer::vulkan::Fence::reset({&fence});
	}


} // namespace se::renderer
