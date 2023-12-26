#pragma once

#include <queue>
#include <map>
#include <mutex>

#include "core.hpp"
#include "event.hpp"
#include "eventListener.hpp"



namespace se
{
	class SE_CORE EventManager
	{
		public:
			static se::UUID addType(se::EventType eventType) SE_THREAD_SAFE;
			static void removeType(se::UUID eventType) SE_THREAD_SAFE;
			template <class T, typename ...Args>
			requires std::is_base_of_v<se::EventListener, T>
			static se::UUID addListener(se::UUID layer, se::UUID eventType, se::UUID linkedObject, Args ...args) SE_THREAD_SAFE;
			static void removeListener(se::UUID listener) SE_THREAD_SAFE;
			static void notify(const se::Event &event) SE_THREAD_SAFE;
			static void flush() SE_THREAD_SAFE;
			static void unload();


		private:
			static std::mutex m_typeMutex;
			static std::mutex m_listenerMutex;
			static std::mutex m_eventMutex;
			static std::map<se::UUID, se::EventType> m_types;
			static std::map<se::UUID, std::map<se::UUID, se::EventListener*>> m_listeners;
			static std::queue<se::Event> m_events;
	};



} // namespace se



#include "eventManager.inl"