#include "se/window/windowManager.hpp"

#include "se/assertion.hpp"
#include "se/engine.hpp"
#include "se/exceptions.hpp"



namespace se::window {
	void WindowManager::load() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			throw se::exceptions::RuntimeError("Can't load SDL2 : " + std::string(SDL_GetError()));
	}


	void WindowManager::unload() {
		s_windows.clear();
		SDL_Quit();
	}


	se::window::Window &WindowManager::createWindow(const se::window::WindowInfos &infos) {
		s_windows.push_back(std::move(se::window::Window(infos)));
		return *s_windows.rbegin();
	}


	se::window::Window &WindowManager::getWindow(se::UUID uuid) {
		for (auto &window : s_windows) {
			if (window.getUUID() == uuid)
				return window;
		}

		SE_ASSERT(false, "Can't find window with the given uuid");
		return *s_windows.begin();
	}


	se::window::Window &WindowManager::getWindow(const std::string &title) {
		for (auto &window : s_windows) {
			if (window.getInfos().title == title)
				return window;
		}

		SE_ASSERT(false, "Can't find window with the given title");
		return *s_windows.begin();
	}


	se::window::Window &WindowManager::getWindow(SDL_Window *_window) {
		for (auto &window : s_windows) {
			if (window.getInternalObject() == _window)
				return window;
		}

		SE_ASSERT(false, "Can't find window with the given window");
		return *s_windows.begin();
	}


	void WindowManager::destroyWindow(se::UUID uuid) {
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it) {
			if (it->getUUID() != uuid)
				continue;

			if (it->getInfos().flags & se::window::WindowFlags::eMasterWindow)
				se::Engine::shutdown();
			s_windows.erase(it);
			return;
		}

		SE_ASSERT(false, "Can't destroy window with the given uuid because it can't be found");
	}


	void WindowManager::destroyWindow(const se::window::Window &window) {
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it) {
			if (&*it != &window)
				continue;

			if (it->getInfos().flags & se::window::WindowFlags::eMasterWindow)
				se::Engine::shutdown();
			s_windows.erase(it);
			return;
		}

		SE_ASSERT(false, "Can't destroy given window because it can't be found");
	}


	void WindowManager::destroyWindow(SDL_Window *window) {
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it) {
			if (it->getInternalObject() != window)
				continue;

			if (it->getInfos().flags & se::window::WindowFlags::eMasterWindow)
				se::Engine::shutdown();
			s_windows.erase(it);
			return;
		}

		SE_ASSERT(false, "Can't destroy window with the given window because it can't be found");
	}



	std::list<se::window::Window> WindowManager::s_windows {};

} // namespace se::window
