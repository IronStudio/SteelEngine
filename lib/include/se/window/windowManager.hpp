#pragma once

#include <list>

#include "se/core.hpp"
#include "se/window/window.hpp"



namespace se::window {
	class SE_CORE WindowManager {
		public:
			static void load();
			static void unload();

			static se::window::Window &createWindow(const se::window::WindowInfos &infos);

			static se::window::Window &getWindow(se::UUID uuid);
			static se::window::Window &getWindow(const std::string &title);
			static se::window::Window &getWindow(SDL_Window *window);

			static bool hasWindow(se::UUID uuid);
			static bool hasWindow(SDL_Window *window);

			static void destroyWindow(se::UUID uuid);
			static void destroyWindow(const se::window::Window &window);
			static void destroyWindow(SDL_Window *window);

		private:
			static std::list<se::window::Window> s_windows;
	};

} // namespace se::window
