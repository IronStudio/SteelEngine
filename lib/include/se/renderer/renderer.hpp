#pragma once

#include "se/application.hpp"
#include "se/core.hpp"
#include "se/renderer/graphicsApi.hpp"
#include "se/window/window.hpp"



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

			const se::renderer::RendererInfos &getInfos() const noexcept {return m_infos;}


		private:
			se::renderer::RendererInfos m_infos;
	};

} // namespace se::renderer
