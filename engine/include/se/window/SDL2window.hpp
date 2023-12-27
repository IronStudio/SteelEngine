#pragma once

#include <SDL2/SDL.h>

#include "window.hpp"



namespace se
{
	class SE_CORE SDL2Window : public se::Window
	{
		public:
			SDL2Window(const se::WindowInfos &infos);
			virtual ~SDL2Window() override;

			void *getWindow() const noexcept override;
			void toggleFullscreen() override;
			void setSize(const glm::ivec2 size) override;
			void setMinSize(const glm::ivec2 size) override;
			void setMaxSize(const glm::ivec2 size) override;
			void toggleResizablity() override;
			void toggleBorder() override;
			void toggleAlwaysOnTop() override;
			void setTitle(const std::string &title) override;
			void setOpacity(float opacity) override;

			void updateInfos() override;


		private:
			SDL_Window *m_window;
	};



} // namespace se
