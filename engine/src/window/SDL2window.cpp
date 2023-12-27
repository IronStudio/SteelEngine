#include "window/SDL2window.hpp"

#include <map>

#include "config/config.hpp"



namespace se
{
	constexpr se::Uint32 fullscreenMode {se::config::useRealFullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_FULLSCREEN_DESKTOP};


	SDL2Window::SDL2Window(const se::WindowInfos &infos) :
		se::Window(infos),
		m_window {nullptr}
	{
		static const std::map<se::GraphicsApi, se::Uint32> apiToFlagMap {
			{se::GraphicsApi::eOpenGL, SDL_WINDOW_OPENGL},
			{se::GraphicsApi::eVulkan, SDL_WINDOW_VULKAN}
		};

		auto apiFlag {apiToFlagMap.find(this->m_infos.graphicsApi)};
		if (apiFlag == apiToFlagMap.end())
			throw std::invalid_argument("Can't create SDL2Window with graphics api '" + se::stringFromGraphicsApi(infos.graphicsApi) + "'");

		se::Uint32 windowFlags {
			SDL_WINDOW_SHOWN |
			apiFlag->second |
			this->m_infos.fullscreen ? fullscreenMode : 0 |
			this->m_infos.resizable ? SDL_WINDOW_RESIZABLE : 0
		};

		m_window = SDL_CreateWindow(
			this->m_infos.title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			this->m_infos.size.x, this->m_infos.size.y,
			windowFlags
		);
		if (m_window == nullptr)
			throw std::runtime_error("Can't create the SDL2 window : " + std::string(SDL_GetError()));
	}



	SDL2Window::~SDL2Window()
	{
		if (m_window != nullptr)
			SDL_DestroyWindow(m_window);
	}



	void *SDL2Window::getWindow() const noexcept
	{
		return m_window;
	}



	void SDL2Window::toggleFullscreen()
	{
		if (SDL_SetWindowFullscreen(m_window, m_infos.fullscreen ? 0 : fullscreenMode) != 0)
			throw std::runtime_error("Can't toggle the SDL2 window : " + std::string(SDL_GetError()));

		m_infos.fullscreen = !m_infos.fullscreen;
	}



} // namespace se
