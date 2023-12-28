#include "window.hpp"



namespace se
{
	se::UUID Window::getUUID() const noexcept
	{
		return m_uuid;
	}



	const se::WindowInfos &Window::getInfos() const noexcept
	{
		return m_infos;
	}



	const std::map<se::InputType, se::UUID> &Window::getEventTypes() const noexcept
	{
		return m_eventTypes;
	}



} // namespace se
