#pragma once

#include <functional>
#include <map>
#include <string>

#include "se/core.hpp"
#include "se/uuid.hpp"

#include "se/renderer/attributeBufferView.hpp"
#include "se/renderer/buffer.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/graphicsApi.hpp"
#include "se/renderer/imageBuffer.hpp"
#include "se/renderer/pipeline.hpp"
#include "se/renderer/shader.hpp"
#include "se/renderer/vertexBufferView.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se {
	template <typename T>
	struct Resource {
		se::UUID uuid {0};
		T *res {nullptr};

		inline ~Resource();
	};

	class SE_CORE ResourceManager {
		public:
			using RendererAttributeBufferView = se::renderer::AttributeBufferViewInfos;
			using RendererBuffer = se::renderer::BufferInfos;
			using RendererBufferTransferor = se::renderer::BufferTransferorInfos;
			struct RendererContext : public se::renderer::ContextInfos {
				se::renderer::GraphicsApi api;
			};
			using RendererImageBuffer = se::renderer::ImageBufferInfos;
			struct RendererGraphicsPipeline {
				se::renderer::Context *context;
				se::renderer::VertexBufferView *vertexBufferView;
				std::vector<se::renderer::AttributeBufferView*> attributeBufferView;
				std::vector<se::renderer::Shader*> shaders;
				std::vector<se::renderer::Format> colorAttachmentFormats;
				se::renderer::Format depthAttachmentFormat;
				se::renderer::Format stencilAttachmentFormat;
				se::renderer::BlendMode blendMode {se::renderer::BlendMode::eSrcAlpha};
			};
			struct RendererComputePipeline {
				se::renderer::Context *context;
				std::vector<se::renderer::Shader*> shaders;
				std::vector<se::renderer::AttributeBufferView*> attributeBufferView;
			};
			using RendererShader = se::renderer::ShaderInfos;
			using RendererVertexBufferView = se::renderer::VertexBufferViewInfos;
			using RendererVramAllocator = se::renderer::VramAllocatorInfos;

			static void load();
			static void unload();

			static se::Resource<se::renderer::AttributeBufferView> load(const RendererAttributeBufferView &infos);
			static se::Resource<se::renderer::Buffer>              load(const RendererBuffer &infos);
			static se::Resource<se::renderer::BufferTransferor>    load(const RendererBufferTransferor &infos);
			static se::Resource<se::renderer::Context>             load(const RendererContext &infos);
			static se::Resource<se::renderer::ImageBuffer>         load(const RendererImageBuffer &infos);
			static se::Resource<se::renderer::Pipeline>            load(const RendererGraphicsPipeline &infos);
			static se::Resource<se::renderer::Pipeline>            load(const RendererComputePipeline &infos);
			static se::Resource<se::renderer::Shader>              load(const RendererShader &infos);
			static se::Resource<se::renderer::VertexBufferView>    load(const RendererVertexBufferView &infos);
			static se::Resource<se::renderer::VramAllocator>       load(const RendererVramAllocator &infos);

			template <typename T>
			static void unload(const se::Resource<T> &res);

		private:
			template <typename T>
			using CreateFuncMap = std::map<se::renderer::GraphicsApi, std::function<T>>;

			static CreateFuncMap<se::renderer::AttributeBufferView*(const se::renderer::AttributeBufferViewInfos&)> s_rendererAttributeBufferViewCreate;
			static CreateFuncMap<se::renderer::Buffer*(const se::renderer::BufferInfos&)>                           s_rendererBufferCreate;
			static CreateFuncMap<se::renderer::BufferTransferor*(const se::renderer::BufferTransferorInfos&)>       s_rendererBufferTransferorCreate;
			static CreateFuncMap<se::renderer::Context*(const se::renderer::ContextInfos&)>                         s_rendererContextCreate;
			static CreateFuncMap<se::renderer::ImageBuffer*(const se::renderer::ImageBufferInfos&)>                 s_rendererImageBufferCreate;
			static CreateFuncMap<se::renderer::Pipeline*(const se::renderer::PipelineInfos&)>                       s_rendererPipelineCreate;
			static CreateFuncMap<se::renderer::Shader*(const se::renderer::ShaderInfos&)>                           s_rendererShaderCreate;
			static CreateFuncMap<se::renderer::VertexBufferView*(const se::renderer::VertexBufferViewInfos&)>       s_rendererVertexBufferViewCreate;
			static CreateFuncMap<se::renderer::VramAllocator*(const se::renderer::VramAllocatorInfos&)>             s_rendererVramAllocatorCreate;
	};

} // namespace se


#include "se/resourceManager.inl"