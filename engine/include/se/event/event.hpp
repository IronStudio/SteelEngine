#pragma once

#include <memory>
#include <string>
#include <variant>

#include "../uuid.hpp"



namespace se
{
	enum class EventPriority
	{
		eBlocking,
		eCanWait
	};



	struct EventType
	{
		se::UUID uuid;
		std::string name;
	};



	struct Event
	{
		std::variant<se::UUID, se::EventType*> type;
		std::shared_ptr<void> data;
		se::UUID object;
		se::EventPriority priority;
	};



} // namespace se
