#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "se/core.hpp"
#include "se/renderer/context.hpp"



namespace se::renderer::opengl {
	class SE_CORE Context final : public se::renderer::Context {
		public:
			Context(const se::renderer::ContextInfos &infos);
			~Context() override;

			void handleWindowSizeChanged() override;
			inline se::renderer::GraphicsApi getGraphicsApi() override {return se::renderer::GraphicsApi::eOpenGL;}

		private:
			static void APIENTRY s_debugMessageCallback(
				GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *msg,
				const void *userData
			);

			SDL_GLContext m_context;
	};

} // namespace se::renderer::opengl
