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



	class SE_CORE EventListener
	{
		public:
			EventListener(se::UUID eventType, se::UUID linkedObject);
			virtual ~EventListener() = default;

			virtual void process(const se::Event &event) SE_THREAD_SAFE = 0;


		protected:
			se::EventListenerInfos m_infos;
	};



} // namespace se
