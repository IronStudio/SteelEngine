#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../core.hpp"
#include "../status.hpp"
#include "../uuid.hpp"
#include "event.hpp"



namespace se
{
	struct ListenerInfos
	{
		se::UUID uuid;
		std::string name;
		std::variant<se::UUID, se::EventType*> type;
		std::optional<std::vector<se::UUID>> objects;
		std::optional<se::UUID> layer;
	};



	class SE_CORE Listener
	{
		public:
			Listener(const se::ListenerInfos &infos);
			virtual ~Listener();

			virtual se::Status onMount() = 0;
			virtual se::Status onProcess(se::EventType eventType, se::Event event) = 0;
			virtual void onUnmount() = 0;

			inline const se::ListenerInfos &getInfos() const noexcept;


		protected:
			se::ListenerInfos m_infos;
	};



} // namespace se



#include "listener.inl"