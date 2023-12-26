#include "eventManager.hpp"

#include "layer.hpp"
#include "workManager.hpp"



struct EventWorkData
{
	se::EventType type;
	se::Event event;
	se::EventListener *listener;
};


namespace se
{
	std::mutex EventManager::m_typeMutex {};
	std::mutex EventManager::m_listenerMutex {};
	std::mutex EventManager::m_eventMutex {};
	std::map<se::UUID, se::EventType> EventManager::m_types {};
	std::map<se::UUID, std::map<se::UUID, se::EventListener*>> EventManager::m_listeners {};
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
		for (auto &listeners : m_listeners)
		{
			auto it {listeners.second.find(listener)};
			if (it == listeners.second.end())
				continue;
			delete it->second;
			listeners.second.erase(it);
			return;
		}
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
			{
				std::lock_guard<std::mutex> _ {m_listenerMutex};
				se::LayerManager::forEachEnabledLayer([&] (const se::LayerInfos &layerInfos) -> bool {
					if (m_listeners[layerInfos.uuid].size() == 0)
						return false;
						
					for (const auto &listener : m_listeners[layerInfos.uuid])
					{
						if (listener.second->getInfos().eventType == type.uuid)
							listener.second->process(type, event);
					}
					return true;
				});
				
				break;
			}


			case se::EventPriority::eNow:
			{
				std::lock_guard<std::mutex> _ {m_listenerMutex};
				se::LayerManager::forEachEnabledLayer([&] (const se::LayerInfos &layerInfos) -> bool {
					if (m_listeners[layerInfos.uuid].size() == 0)
						return false;
						
					for (const auto &listener : m_listeners[layerInfos.uuid])
					{
						if (listener.second->getInfos().eventType == type.uuid)
						{
							se::WorkInfos workInfos {};
							workInfos.priority = se::WorkPriority::eHigh;
							workInfos.data = EventWorkData(
								type,
								event,
								listener.second
							);
							workInfos.work = [] (se::WorkInfos workInfos) -> se::Status {
								auto data {std::any_cast<EventWorkData> (workInfos.data)};
								data.listener->process(data.type, data.event);
								return se::Status::eSuccess;
							};
							se::WorkManager::addWork(workInfos);
						}
					}
					return true;
				});
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



	void EventManager::flush() SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_eventMutex};
		std::atomic<se::Int32> eventCount {0};
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
				se::LayerManager::forEachEnabledLayer([&] (const se::LayerInfos &layerInfos) -> bool {
					if (m_listeners[layerInfos.uuid].size() == 0)
						return false;
						
					for (const auto &listener : m_listeners[layerInfos.uuid])
					{
						if (listener.second->getInfos().eventType == type.uuid)
						{
							++eventCount;
							se::WorkInfos workInfos {};
							workInfos.priority = se::WorkPriority::eHigh;
							workInfos.data = EventWorkData(
								type,
								m_events.front(),
								listener.second
							);
							workInfos.work = [&eventCount] (se::WorkInfos workInfos) -> se::Status {
								auto data {std::any_cast<EventWorkData> (workInfos.data)};
								data.listener->process(data.type, data.event);
								--eventCount;
								return se::Status::eSuccess;
							};
							se::WorkManager::addWork(workInfos);
						}
					}

					return true;
				});
			}

			m_events.pop();
		}

		while (eventCount != 0);
	}



	void EventManager::unload()
	{
		for (const auto &listeners : m_listeners)
		{
			for (const auto &listener : listeners.second)
				delete listener.second;
		}
	}



} // namespace se
