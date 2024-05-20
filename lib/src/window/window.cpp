#include "se/window/window.hpp"

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::window {
	Window::Window() :
		m_infos {},
		m_window {nullptr},
		m_uuid {0}
	{

	}


	Window::~Window() {
		if (m_window != nullptr)
			SDL_DestroyWindow(m_window);
	}


	Window::Window(const se::window::WindowInfos &infos) :
		m_infos {infos},
		m_window {nullptr},
		m_uuid {}
	{
		SE_ASSERT(m_infos.size.x > 0 && m_infos.size.y > 0, "Both component of size must be greater than 0");

		if (m_infos.position.x == se::window::centerPosition)
			m_infos.position.x = SDL_WINDOWPOS_CENTERED;
		if (m_infos.position.y == se::window::centerPosition)
			m_infos.position.y = SDL_WINDOWPOS_CENTERED;
		if (m_infos.position.x == se::window::undefinedPosition)
			m_infos.position.x = SDL_WINDOWPOS_UNDEFINED;
		if (m_infos.position.y == se::window::undefinedPosition)
			m_infos.position.y = SDL_WINDOWPOS_UNDEFINED;

		se::Uint32 flags {0};
		switch (m_infos.graphicsApi) {
			case se::renderer::GraphicsApi::eOpenGL:
				flags |= SDL_WINDOW_OPENGL;
				break;
			case se::renderer::GraphicsApi::eVulkan:
				flags |= SDL_WINDOW_VULKAN;
				break;
			case se::renderer::GraphicsApi::eMetal:
				flags |= SDL_WINDOW_METAL;
				break;
		}

		if (m_infos.flags & se::window::WindowFlags::eResizable)
			flags |= SDL_WINDOW_RESIZABLE;

		m_window = SDL_CreateWindow(m_infos.title.c_str(), m_infos.position.x, m_infos.position.y, m_infos.size.x, m_infos.size.y, flags);
		if (m_window == nullptr)
			throw se::exceptions::RuntimeError("Can't create window " + std::string(m_uuid) + " : " + std::string(SDL_GetError()));

		SDL_GetWindowSize(m_window, &m_infos.size.x, &m_infos.size.y);
		SDL_GetWindowPosition(m_window, &m_infos.position.x, &m_infos.position.y);

		if (m_infos.minSize.x == se::window::sizeLimitFit)
			m_infos.minSize.x = m_infos.size.x;
		else if (m_infos.minSize.x == se::window::undefinedLimitSize)
			m_infos.minSize.x = 0;
		if (m_infos.minSize.y == se::window::sizeLimitFit)
			m_infos.minSize.y = m_infos.size.y;
		else if (m_infos.minSize.y == se::window::undefinedLimitSize)
			m_infos.minSize.y = 0;
		SDL_SetWindowMinimumSize(m_window, m_infos.minSize.x, m_infos.minSize.y);

		if (m_infos.maxSize.x == se::window::sizeLimitFit)
			m_infos.maxSize.x = m_infos.size.x;
		else if (m_infos.maxSize.x == se::window::undefinedLimitSize)
			m_infos.maxSize.x = 0;
		if (m_infos.maxSize.y == se::window::sizeLimitFit)
			m_infos.maxSize.y = m_infos.size.y;
		else if (m_infos.maxSize.y == se::window::undefinedLimitSize)
			m_infos.maxSize.y = 0;
		SDL_SetWindowMaximumSize(m_window, m_infos.maxSize.x, m_infos.maxSize.y);

		SDL_GetWindowMinimumSize(m_window, &m_infos.minSize.x, &m_infos.minSize.y);
		SDL_GetWindowMaximumSize(m_window, &m_infos.maxSize.x, &m_infos.maxSize.y);
	}


	Window::Window(se::window::Window &&window) noexcept :
		m_infos {std::move(window.m_infos)},
		m_window {window.m_window},
		m_uuid {window.m_uuid}
	{
		window.m_window = nullptr;
		window.m_uuid = 0;
	}


	const se::window::Window &Window::operator=(se::window::Window &&window) noexcept {
		m_infos = std::move(window.m_infos);
		m_window = window.m_window;
		m_uuid = window.m_uuid;

		window.m_window = nullptr;
		window.m_uuid = 0;

		return *this;
	}


	void Window::resize(const se::Vec2i &size) {
		SE_ASSERT(size.x > 0 && size.y > 0, "Both component of size must be greater than 0");
		SDL_SetWindowSize(m_window, size.x, size.y);
	}


	void Window::move(se::Vec2i position) {
		if (position.x == se::window::centerPosition)
			position.x = SDL_WINDOWPOS_CENTERED;
		if (position.y == se::window::centerPosition)
			position.y = SDL_WINDOWPOS_CENTERED;
		if (position.x == se::window::undefinedPosition)
			position.x = SDL_WINDOWPOS_UNDEFINED;
		if (position.y == se::window::undefinedPosition)
			position.y = SDL_WINDOWPOS_UNDEFINED;
		
		SDL_SetWindowPosition(m_window, position.x, position.y);
	}


	void Window::setTitle(const std::string &title) {
		SDL_SetWindowTitle(m_window, title.c_str());
	}


	void Window::sync(se::window::WindowResyncMask mask) {
		if (mask & se::window::WindowResync::eSize)
			SDL_GetWindowSize(m_window, &m_infos.size.x, &m_infos.size.y);
		if (mask & se::window::WindowResync::ePosition)
			SDL_GetWindowPosition(m_window, &m_infos.position.x, &m_infos.position.y);
	}


} // namespace se::window
