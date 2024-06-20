#pragma once

#include "se/application.hpp"
#include "se/core.hpp"
#include "se/resourceManager.hpp"
#include "se/window/window.hpp"

#include "se/renderer/context.hpp"
#include "se/renderer/graphicsApi.hpp"



namespace se::renderer {
	struct RendererInfos {
		se::Application *application;
		se::window::Window *window;
		se::renderer::GraphicsApi api;
	};

	class SE_CORE Renderer {
		public:
			Renderer(const se::renderer::RendererInfos &infos);
			~Renderer();

			void render();

			inline const se::renderer::RendererInfos &getInfos() const noexcept {return m_infos;}
			inline se::renderer::Context *getContext() const noexcept {return m_context.res;}


		private:
			se::renderer::RendererInfos m_infos;
			se::Resource<se::renderer::Context> m_context;
			se::Resource<se::renderer::Shader> m_computeShader;
			se::Resource<se::renderer::VramAllocator> m_gpuOnlyAllocator;
			se::Resource<se::renderer::VramAllocator> m_stagingAllocator;
			se::Resource<se::renderer::AttributeBufferView> m_worldMapBufferView;
			se::Resource<se::renderer::AttributeBufferView> m_cameraBufferView;
			se::Resource<se::renderer::AttributeBufferView> m_hittedBlockBufferView;
			se::Resource<se::renderer::Buffer> m_worldMapBuffer;
			se::Resource<se::renderer::Buffer> m_cameraBuffer;
			se::Resource<se::renderer::Buffer> m_hittedBlocksBuffer;
			se::Resource<se::renderer::Buffer> m_cameraStagingBuffer;
			se::Resource<se::renderer::Pipeline> m_computePipeline;
	};

} // namespace se::renderer
