#pragma once

#include "core.hpp"
#include "event.hpp"



namespace se
{
	struct EventListenerInfos
	{
		se::UUID uuid;
		se::UUID eventType;
		se::UUID linkedObject;
	};



	/**
	 * @brief Base class of all events listeners 
	*/
	class SE_CORE EventListener
	{
		public:
			EventListener(se::UUID eventType, se::UUID linkedObject);
			virtual ~EventListener() = default;

			virtual void process(se::EventType type, se::Event event) SE_THREAD_SAFE = 0;

			inline const se::EventListenerInfos &getInfos() const noexcept;


		protected:
			se::EventListenerInfos m_infos;
	};



} // namespace se



#include "eventListener.inl"