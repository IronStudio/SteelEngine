#include "se/window/window.hpp"



namespace se::window {
	const se::window::WindowInfos &Window::getInfos() const noexcept {
		return m_infos;
	}

	SDL_Window *Window::getInternalObject() const noexcept {
		return m_window;
	}

	se::UUID Window::getUUID() const noexcept {
		return m_uuid;
	}

} // namespace se::window
