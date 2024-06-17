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
	};

} // namespace se::renderer
