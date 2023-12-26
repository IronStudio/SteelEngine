#include "eventManager.hpp"



namespace se
{
	template <class T, typename ...Args>
	requires std::is_base_of_v<se::EventListener, T>
	se::UUID EventManager::addListener(se::UUID layer, se::UUID eventType, se::UUID linkedObject, Args ...args) SE_THREAD_SAFE
	{
		se::EventListener *listener {new T(eventType, linkedObject, args...)};
		
		std::lock_guard<std::mutex> _ {m_listenerMutex};
		m_listeners[layer][listener->getInfos().uuid] = listener;
		return listener->getInfos().uuid;
	}



} // namespace se
