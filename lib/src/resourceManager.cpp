#include "se/resourceManager.hpp"

#include "se/logger.hpp"

#include "se/renderer/opengl/context.hpp"

#include "se/renderer/vulkan/attributeBufferView.hpp"
#include "se/renderer/vulkan/buffer.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/imageBuffer.hpp"
#include "se/renderer/vulkan/pipeline.hpp"
#include "se/renderer/vulkan/shader.hpp"
#include "se/renderer/vulkan/vertexBufferView.hpp"
#include "se/renderer/vulkan/vramAllocator.hpp"



namespace se {

	void ResourceManager::load() {

	}


	void ResourceManager::unload() {

	}


	se::Resource<se::renderer::Context> ResourceManager::load(const RendererContext &infos) {
		se::Resource<se::renderer::Context> resource {};
		resource.uuid = {};
		resource.res = s_rendererContextCreate[infos.api](infos);
		return resource;
	}


	#define LOAD(T, createFunc) se::Resource<se::renderer::T> resource {};\
		resource.uuid = {};\
		resource.res = createFunc[infos.context->getGraphicsApi()](infos);\
		return resource

	se::Resource<se::renderer::AttributeBufferView> ResourceManager::load(const RendererAttributeBufferView &infos) {
		LOAD(AttributeBufferView, s_rendererAttributeBufferViewCreate);
	}


	se::Resource<se::renderer::Buffer> ResourceManager::load(const RendererBuffer &infos) {
		LOAD(Buffer, s_rendererBufferCreate);
	}


	se::Resource<se::renderer::BufferTransferor> ResourceManager::load(const RendererBufferTransferor &infos) {
		LOAD(BufferTransferor, s_rendererBufferTransferorCreate);
	}


	se::Resource<se::renderer::ImageBuffer> ResourceManager::load(const RendererImageBuffer &infos) {
		LOAD(ImageBuffer, s_rendererImageBufferCreate);
	}


	se::Resource<se::renderer::Pipeline> ResourceManager::load(const RendererGraphicsPipeline &infos) {
		se::Resource<se::renderer::Pipeline> resource {};
		resource.uuid = {};
		se::renderer::PipelineInfos pipelineInfos {};
		pipelineInfos.context = infos.context;
		pipelineInfos.vertexBufferView = infos.vertexBufferView;
		pipelineInfos.attributeBufferView = infos.attributeBufferView;
		pipelineInfos.shaders = infos.shaders;
		pipelineInfos.colorAttachmentFormats = infos.colorAttachmentFormats;
		pipelineInfos.depthAttachmentFormat = infos.depthAttachmentFormat;
		pipelineInfos.stencilAttachmentFormat = infos.stencilAttachmentFormat;
		pipelineInfos.blendMode = infos.blendMode;
		pipelineInfos.type = se::renderer::PipelineType::eRasterization;
		resource.res = s_rendererPipelineCreate[infos.context->getGraphicsApi()](pipelineInfos);
		return resource;
	}


	se::Resource<se::renderer::Pipeline> ResourceManager::load(const RendererComputePipeline &infos) {
		se::Resource<se::renderer::Pipeline> resource {};
		resource.uuid = {};
		se::renderer::PipelineInfos pipelineInfos {};
		pipelineInfos.context = infos.context;
		pipelineInfos.shaders = infos.shaders;
		pipelineInfos.attributeBufferView = infos.attributeBufferView;
		pipelineInfos.type = se::renderer::PipelineType::eCompute;
		resource.res = s_rendererPipelineCreate[infos.context->getGraphicsApi()](pipelineInfos);
		return resource;
	}


	se::Resource<se::renderer::Shader> ResourceManager::load(const RendererShader &infos) {
		LOAD(Shader, s_rendererShaderCreate);
	}


	se::Resource<se::renderer::VertexBufferView> ResourceManager::load(const RendererVertexBufferView &infos) {
		LOAD(VertexBufferView, s_rendererVertexBufferViewCreate);
	}


	se::Resource<se::renderer::VramAllocator> ResourceManager::load(const RendererVramAllocator &infos) {
		LOAD(VramAllocator, s_rendererVramAllocatorCreate);
	}




	#define STRAIGHT_REPLACEMENT(api, Infos, Type) {se::renderer::GraphicsApi::api, [](const se::renderer::Infos &infos) {\
		return new se::renderer::Type(infos);\
	}}

	ResourceManager::CreateFuncMap<se::renderer::Context*(const se::renderer::ContextInfos &)> ResourceManager::s_rendererContextCreate {
		STRAIGHT_REPLACEMENT(eVulkan, ContextInfos, vulkan::Context),
		STRAIGHT_REPLACEMENT(eOpenGL, ContextInfos, opengl::Context),
	};

	ResourceManager::CreateFuncMap<
		se::renderer::AttributeBufferView*(const se::renderer::AttributeBufferViewInfos &)
	> ResourceManager::s_rendererAttributeBufferViewCreate {
		STRAIGHT_REPLACEMENT(eVulkan, AttributeBufferViewInfos, vulkan::AttributeBufferView)
	};

	ResourceManager::CreateFuncMap<se::renderer::Buffer*(const se::renderer::BufferInfos &)> ResourceManager::s_rendererBufferCreate {
		STRAIGHT_REPLACEMENT(eVulkan, BufferInfos, vulkan::Buffer)
	};

	ResourceManager::CreateFuncMap<
		se::renderer::BufferTransferor*(const se::renderer::BufferTransferorInfos &)
	> ResourceManager::s_rendererBufferTransferorCreate {
		STRAIGHT_REPLACEMENT(eVulkan, BufferTransferorInfos, vulkan::BufferTransferor)
	};

	ResourceManager::CreateFuncMap<se::renderer::ImageBuffer*(const se::renderer::ImageBufferInfos &)> ResourceManager::s_rendererImageBufferCreate {
		STRAIGHT_REPLACEMENT(eVulkan, ImageBufferInfos, vulkan::ImageBuffer)
	};

	ResourceManager::CreateFuncMap<se::renderer::Pipeline*(const se::renderer::PipelineInfos&)> ResourceManager::s_rendererPipelineCreate {
		STRAIGHT_REPLACEMENT(eVulkan, PipelineInfos, vulkan::Pipeline)
	};

	ResourceManager::CreateFuncMap<se::renderer::Shader*(const se::renderer::ShaderInfos&)> ResourceManager::s_rendererShaderCreate {
		STRAIGHT_REPLACEMENT(eVulkan, ShaderInfos, vulkan::Shader)
	};

	ResourceManager::CreateFuncMap<
		se::renderer::VertexBufferView*(const se::renderer::VertexBufferViewInfos&)
	> ResourceManager::s_rendererVertexBufferViewCreate {
		STRAIGHT_REPLACEMENT(eVulkan, VertexBufferViewInfos, vulkan::VertexBufferView)
	};

	ResourceManager::CreateFuncMap<
		se::renderer::VramAllocator*(const se::renderer::VramAllocatorInfos&)
	> ResourceManager::s_rendererVramAllocatorCreate {
		STRAIGHT_REPLACEMENT(eVulkan, VramAllocatorInfos, vulkan::VramAllocator)
	};

} // namespace se
