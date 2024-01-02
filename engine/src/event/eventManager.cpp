#include "event/eventManager.hpp"

#include "layer.hpp"



namespace se
{
	std::list<se::EventType> EventManager::s_types {};
	std::map<const se::EventType*, std::list<std::shared_ptr<se::Listener>>> EventManager::s_listeners {};
	std::queue<se::Event> EventManager::s_waitingEvents {};



	se::Status EventManager::onLoad()
	{
		return se::Status::eSuccess;
	}



	void EventManager::onUnload()
	{
		s_listeners.clear();
		s_types.clear();
	}



	se::UUID EventManager::addType(const se::EventType &type)
	{
		s_types.push_back(type);
		auto it {s_types.rbegin()};
		it->uuid = se::UUIDManager::generate<se::EventType> (&*it);
		return it->uuid;
	}



	void EventManager::removeType(se::UUID type)
	{
		for (auto it {s_types.cbegin()}; it != s_types.cend(); ++it)
		{
			if (it->uuid != type)
				continue;

			auto list {s_listeners.find(&*it)};
			s_listeners.erase(list);
			s_types.erase(it);
			return;
		}

		throw std::invalid_argument("Can't remove unknown event type");
	}



	void EventManager::removeListener(se::UUID listener)
	{
		for (auto list {s_listeners.begin()}; list != s_listeners.end(); ++list)
		{
			for (auto it {list->second.cbegin()}; it != list->second.cend(); ++it)
			{
				if ((*it)->getInfos().uuid != listener)
					continue;

				(*it)->onUnmount();
				list->second.erase(it);
				return;
			}
		}

		throw std::invalid_argument("Can't remove unknown listener");
	}



	void EventManager::notify(const se::Event &event)
	{
		switch (event.priority)
		{
			case se::EventPriority::eBlocking:
			{
				se::EventManager::s_sendEvents(event);
				break;
			}

			case se::EventPriority::eCanWait:
			{
				s_waitingEvents.push(event);
				break;
			}

			default:
				throw std::invalid_argument("Can't notify event of invalid priority");
				break;
		}
	}



	void EventManager::flush()
	{
		while (!s_waitingEvents.empty())
		{
			se::EventManager::s_sendEvents (s_waitingEvents.front());
			s_waitingEvents.pop();
		}
	}



	void EventManager::s_sendEvents(const se::Event &event)
	{
		const se::EventType *type {nullptr};
		if (std::holds_alternative<se::UUID> (event.type))
		{
			se::UUID typeUUID {std::get<se::UUID> (event.type)};
			for (auto it {s_types.cbegin()}; it != s_types.cend(); ++it)
			{
				if (it->uuid != typeUUID)
					continue;

				type = &*it;
				break;
			}
		}

		else
			type = std::get<se::EventType*> (event.type);

		auto it {s_listeners.find(type)};
		if (it == s_listeners.end())
			return;

		auto &listeners {it->second};


		std::optional<se::Uint8> levelThreshold {};

		std::map<se::UUID, const se::LayerInfos*> layers {};

		for (auto layer {se::LayerManager::cbegin()}; layer != se::LayerManager::cend(); ++layer)
			layers[(*layer)->uuid] = *layer;


		for (const auto &listener : listeners)
		{
			if (!levelThreshold.has_value())
			{
				if (listener->getInfos().layer.has_value())
				{
					auto layer {layers.find(listener->getInfos().layer.value())};
					if (layer != layers.end())
						levelThreshold = layer->second->level;
				}
				continue;
			}

			if (!listener->getInfos().layer.has_value())
				continue;

			auto layer {layers.find(listener->getInfos().layer.value())};
			if (layer == layers.end())
				continue;

			if (levelThreshold > layer->second->level)
				levelThreshold = layer->second->level;
		}

		std::list<std::list<std::shared_ptr<se::Listener>>::iterator> listenersToRemove {};

		for (auto listener {listeners.begin()}; listener != listeners.end(); ++listener)
		{
			if (!(*listener)->getInfos().layer.has_value())
			{
				if ((int)((*listener)->onProcess(*type, event)) & (int)se::Status::eRemoveListener)
					listenersToRemove.push_back(listener);
				continue;
			}

			auto layer {layers.find((*listener)->getInfos().layer.value())};
			if (layer == layers.end())
				continue;

			if (layer->second->level > levelThreshold)
				continue;
				
			if ((int)((*listener)->onProcess(*type, event)) & (int)se::Status::eRemoveListener)
				listenersToRemove.push_back(listener);
		}


		for (auto &toRemove : listenersToRemove)
			listeners.erase(toRemove);
	}



} // namespace se
