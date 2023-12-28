#include "window/window.hpp"

#include <stdexcept>

#include "eventManager.hpp"



namespace se
{
	Window::Window(const se::WindowInfos &infos) :
		m_uuid {se::UUIDManager::generate<se::Window> ()},
		m_infos {infos},
		m_eventTypes {}
	{
		m_infos.windowedSize = m_infos.size;

		se::EventType eventType {};
		eventType.linkedObject = m_uuid;

		for (se::Uint64 i {0}; i < (se::Uint64)se::InputType::__last; ++i)
			m_eventTypes[(se::InputType)i] = se::EventManager::addType(eventType);
	}



	Window::~Window()
	{
		for (const auto &type : m_eventTypes)
			se::EventManager::removeType(type.second);
	}



} // namespace se
