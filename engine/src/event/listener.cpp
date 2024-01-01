#include "event/listener.hpp"



namespace se
{
	Listener::Listener(const se::ListenerInfos &infos) :
		m_infos {infos}
	{
		m_infos.uuid = se::UUIDManager::generate<se::Listener> (this);
	}



	Listener::~Listener()
	{
		se::UUIDManager::remove(m_infos.uuid);
	}



} // namespace se
