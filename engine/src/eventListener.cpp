#include "eventListener.hpp"

#include "uuid.hpp"



namespace se
{
	EventListener::EventListener(se::UUID eventType, se::UUID linkedObject) :
		m_infos {
			se::UUIDManager::generate<se::EventListener> (),
			eventType,
			linkedObject
		}
	{
		#ifndef NDEBUG
			if (linkedObject != 0 && !se::UUIDManager::isValid(linkedObject))
				throw std::invalid_argument("Can't create event listener on an invalid object");
			
			if (!se::UUIDManager::isValid(eventType))
				throw std::invalid_argument("Can't create event listener on an invalid event type");

			if (se::UUIDManager::getType(eventType) != typeid(se::EventType).hash_code())
				throw std::invalid_argument("`eventType`'s UUID must be an `se::EventType`");
		#endif
	}



	EventListener::~EventListener()
	{
		se::UUIDManager::remove(m_infos.uuid);
	}



} // namespace se
