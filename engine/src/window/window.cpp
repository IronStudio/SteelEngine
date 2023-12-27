#include "window/window.hpp"

#include <stdexcept>



namespace se
{
	Window::Window(const se::WindowInfos &infos) :
		m_uuid {se::UUIDManager::generate<se::Window> ()},
		m_infos {infos}
	{
		m_infos.windowedSize = m_infos.size;
	}



} // namespace se
