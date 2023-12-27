#pragma once

#include <SDL2/SDL.h>

#include "window.hpp"



namespace se
{
	class SDL2Window : public se::Window
	{
		public:
			SDL2Window(const se::WindowInfos &infos);
			virtual ~SDL2Window() override;

			void *getWindow() const noexcept override;
			void toggleFullscreen() override;


		private:
			SDL_Window *m_window;
	};



} // namespace se
