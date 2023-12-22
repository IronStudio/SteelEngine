#include "eventManager.hpp"



namespace se
{
	std::mutex EventManager::m_typeMutex {};
	std::mutex EventManager::m_listenerMutex {};
	std::mutex EventManager::m_eventMutex {};
	std::map<se::UUID, se::EventType> EventManager::m_types {};
	std::map<se::UUID, se::EventListener*> EventManager::m_listeners {};
	std::queue<se::Event> EventManager::m_events {};



	se::UUID EventManager::addType(se::EventType eventType) SE_THREAD_SAFE
	{
		eventType.uuid = se::UUIDManager::generate<se::EventType> ();
		std::lock_guard<std::mutex> _ {m_typeMutex};
		m_types[eventType.uuid] = eventType;
		return eventType.uuid;
	}



	void EventManager::removeType(se::UUID eventType) SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_typeMutex};
		m_types.erase(eventType);
	}



	void EventManager::removeListener(se::UUID listener) SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_listenerMutex};
		auto it {m_listeners.find(listener)};
		if (it == m_listeners.end())
			return;
		delete it->second;
		m_listeners.erase(it);
	}



	void EventManager::notify(const se::Event &event) SE_THREAD_SAFE
	{
		se::EventType type {};

		{
			std::lock_guard<std::mutex> _ {m_typeMutex};
			auto it {m_types.find(event.type)};
			if (it == m_types.end())
				throw std::invalid_argument("Can't notify event of an invalid / undefined type");

			type = it->second;
		}
		
		switch (event.priority)
		{
			case se::EventPriority::eBlocking:
			case se::EventPriority::eNow:
			{
				std::lock_guard<std::mutex> _ {m_listenerMutex};
				for (const auto &listener : m_listeners)
				{
					if (listener.second->getInfos().eventType == type.uuid)
						listener.second->process(type, event);
				}
				break;
			}

			case se::EventPriority::eCanWait:
			{
				std::lock_guard<std::mutex> _ {m_eventMutex};
				m_events.push(event);
				break;
			}

			default:
				throw std::invalid_argument("Can't notify event of invalid priority");
		}
	}



	void EventManager::updateEvents() SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_eventMutex};
		while (!m_events.empty())
		{
			se::EventType type {};
			{
				std::lock_guard<std::mutex> _ {m_typeMutex};
				auto it {m_types.find(m_events.front().type)};
				if (it == m_types.end())
					throw std::invalid_argument("Can't update event of an invalid / undefined type");

				type = it->second;
			}

			{
				std::lock_guard<std::mutex> _ {m_listenerMutex};
				for (const auto &listener : m_listeners)
				{
					if (listener.second->getInfos().eventType == type.uuid)
						listener.second->process(type, m_events.front());
				}
			}

			m_events.pop();
		}
	}



	void EventManager::unload()
	{
		for (const auto &listener : m_listeners)
			delete listener.second;
	}



} // namespace se
