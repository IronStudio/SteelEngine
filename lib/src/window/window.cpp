#include "se/window/window.hpp"

#include "se/exceptions.hpp"



namespace se::window {
#ifdef SE_WSI_SDL3
	SDL3Window::SDL3Window() :
		m_window {nullptr}
	{

	}

	SDL3Window::~SDL3Window() {

	}

	void SDL3Window::create(const se::window::WindowInfos &infos) {
		se::Uint32 flags {0};
		switch (infos.graphicsApi) {
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

		m_window = SDL_CreateWindow(infos.title.c_str(), infos.width, infos.height, flags);
		if (m_window == nullptr)
			throw se::exceptions::RuntimeError("Can't create an SDL3 window : " + std::string(SDL_GetError()));
	}

	void SDL3Window::destroy() {
		if (m_window != nullptr)
			SDL_DestroyWindow(m_window);
	}

	void SDL3Window::move(SDL3Window &&window) {
		m_window = window.m_window;
		window.m_window = nullptr;
	}

#elifdef SE_WSI_SDL2

	SDL2Window::SDL2Window() :
		m_window {nullptr}
	{

	}

	SDL2Window::~SDL2Window() {

	}

	void SDL2Window::create(const se::window::WindowInfos &infos) {
		se::Uint32 flags {0};
		switch (infos.graphicsApi) {
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

		m_window = SDL_CreateWindow(infos.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, infos.width, infos.height, flags);
		if (m_window == nullptr)
			throw se::exceptions::RuntimeError("Can't create an SDL3 window : " + std::string(SDL_GetError()));
	}

	void SDL2Window::destroy() {
		if (m_window != nullptr)
			SDL_DestroyWindow(m_window);
	}

	void SDL2Window::move(SDL2Window &&window) {
		m_window = window.m_window;
		window.m_window = nullptr;
	}

#endif
} // namespace se::window
