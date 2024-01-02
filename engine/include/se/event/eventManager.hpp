#pragma once

#include <list>
#include <map>
#include <queue>

#include "../core.hpp"
#include "../status.hpp"
#include "event.hpp"
#include "listener.hpp"



namespace se
{
	class SE_CORE EventManager
	{
		public:
			static se::Status onLoad();
			static void onUnload();

			static se::UUID addType(const se::EventType &type);
			static void removeType(se::UUID type);

			template <typename T, typename ...Args>
			requires std::is_base_of_v<se::Listener, T>
			static se::UUID addListener(const se::ListenerInfos &infos, Args ...args);
			static void removeListener(se::UUID listener);

			static void notify(const se::Event &event);
			static void flush();


		private:
			static void s_sendEvents(const se::Event &event);

			static std::list<se::EventType> s_types;
			static std::map<const se::EventType*, std::list<std::shared_ptr<se::Listener>>> s_listeners;
			static std::queue<se::Event> s_waitingEvents;
	};

} // namespace se



#include "eventManager.inl"