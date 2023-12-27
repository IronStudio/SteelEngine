#include "window/SDL2window.hpp"

#include <map>
#include <stdexcept>

#include "config/config.hpp"
#include "logging.hpp"



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

		auto apiFlag {apiToFlagMap.find(m_infos.graphicsApi)};
		if (apiFlag == apiToFlagMap.end())
			throw std::invalid_argument("Can't create SDL2Window with graphics api '" + se::stringFromGraphicsApi(infos.graphicsApi) + "'");

		se::Uint32 windowFlags {
			SDL_WINDOW_SHOWN |
			apiFlag->second
		};

		if (m_infos.fullscreen)
			windowFlags |= fullscreenMode;

		if (m_infos.resizable)
			windowFlags |= SDL_WINDOW_RESIZABLE;

		if (m_infos.borderless)
			windowFlags |= SDL_WINDOW_BORDERLESS;

		if (m_infos.alwaysOnTop)
			windowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

		m_window = SDL_CreateWindow(
			m_infos.title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_infos.size.x, m_infos.size.y,
			windowFlags
		);
		if (m_window == nullptr)
			throw std::runtime_error("Can't create the SDL2 window : " + std::string(SDL_GetError()));


		SDL_GetWindowSize(m_window, &m_infos.size.x, &m_infos.size.y);

		if (m_infos.minSize != SE_UNDEFINED_SIZE)
			SDL_SetWindowMinimumSize(m_window, m_infos.minSize.x, m_infos.minSize.y);

		if (m_infos.maxSize != SE_UNDEFINED_SIZE)
			SDL_SetWindowMaximumSize(m_window, m_infos.maxSize.x, m_infos.maxSize.y);

		if (m_infos.opacity != 1.f)
		{
			if (SDL_SetWindowOpacity(m_window, m_infos.opacity) != 0)
				throw std::runtime_error("Can't change window opacity : " + std::string(SDL_GetError()));
		}
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
		SDL_GetWindowSize(m_window, &m_infos.size.x, &m_infos.size.y);
	}



	void SDL2Window::setSize(const glm::ivec2 size)
	{
		m_infos.windowedSize = size;
		if (m_infos.fullscreen)
			return;

		m_infos.size = size;
		SDL_SetWindowSize(m_window, m_infos.size.x, m_infos.size.y);
	}



	void SDL2Window::setMinSize(const glm::ivec2 size)
	{
		m_infos.minSize = size;
		SDL_SetWindowMinimumSize(m_window, m_infos.minSize.x, m_infos.minSize.y);
	}



	void SDL2Window::setMaxSize(const glm::ivec2 size)
	{
		m_infos.maxSize = size;
		SDL_SetWindowMaximumSize(m_window, m_infos.maxSize.x, m_infos.maxSize.y);
	}



	void SDL2Window::toggleResizablity()
	{
		m_infos.resizable = !m_infos.resizable;
		SDL_SetWindowResizable(m_window, (SDL_bool)m_infos.resizable);
	}



	void SDL2Window::toggleBorder()
	{
		m_infos.borderless = !m_infos.borderless;
		SDL_SetWindowBordered(m_window, (SDL_bool)!m_infos.borderless);
	}



	void SDL2Window::toggleAlwaysOnTop()
	{
		m_infos.alwaysOnTop = !m_infos.alwaysOnTop;
		SDL_SetWindowAlwaysOnTop(m_window, (SDL_bool)m_infos.alwaysOnTop);
	}



	void SDL2Window::setTitle(const std::string &title)
	{
		m_infos.title = title;
		SDL_SetWindowTitle(m_window, m_infos.title.c_str());
	}



	void SDL2Window::setOpacity(float opacity)
	{
		if (m_infos.opacity == opacity)
			return;

		m_infos.opacity = opacity;
		if (SDL_SetWindowOpacity(m_window, m_infos.opacity) != 0)
			throw std::runtime_error("Can't change window opacity : " + std::string(SDL_GetError()));
	}



	void SDL2Window::updateInfos()
	{
		SDL_GetWindowSize(m_window, &m_infos.size.x, &m_infos.size.y);
		SDL_GetWindowMinimumSize(m_window, &m_infos.minSize.x, &m_infos.minSize.y);
		SDL_GetWindowMaximumSize(m_window, &m_infos.maxSize.x, &m_infos.maxSize.y);

		if (m_infos.fullscreen)
			return;

		m_infos.windowedSize = m_infos.size;
	}



} // namespace se
