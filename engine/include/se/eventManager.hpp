#pragma once

#include <queue>
#include <map>
#include <mutex>

#include "core.hpp"
#include "event.hpp"
#include "eventListener.hpp"



namespace se
{
	/**
	 * @brief A static global class that handles the event system, that is the system that permits to the rest of the
	 *        engine / app's systems to communicate
	*/
	class SE_CORE EventManager
	{
		public:
			/**
			 * @brief Add a new type of event that can be listened
			*/
			static se::UUID addType(se::EventType eventType) SE_THREAD_SAFE;
			static void removeType(se::UUID eventType) SE_THREAD_SAFE;
			/**
			 * @brief Add a new listener to the given event type and layer
			 * @tparam T The type of the listener to create. Must be a child class of `se::EventListener`
			*/
			template <class T, typename ...Args>
			requires std::is_base_of_v<se::EventListener, T>
			static se::UUID addListener(se::UUID layer, se::UUID eventType, se::UUID linkedObject, Args ...args) SE_THREAD_SAFE;
			static void removeListener(se::UUID listener) SE_THREAD_SAFE;
			/**
			 * @brief Launch an event of the choosen type, that the listeners will listen to
			*/
			static void notify(const se::Event &event) SE_THREAD_SAFE;
			/**
			 * @brief Call it once per frame to flush all non-urgent events
			*/
			static void flush() SE_THREAD_SAFE;
			/**
			 * @brief Release memory of all listeners
			*/
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