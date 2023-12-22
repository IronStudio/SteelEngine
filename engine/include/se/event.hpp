#pragma once

#include <any>

#include "time.hpp"
#include "uuid.hpp"



namespace se
{
	enum class EventPriority
	{
		eBlocking,
		eNow,
		eCanWait
	};



	struct EventType
	{
		se::UUID uuid;
		se::UUID linkedObject;
	};



	struct Event
	{
		se::UUID uuid;
		se::UUID type;
		std::any data;
		se::Frame lifeExpectancy;
		se::EventPriority priority {se::EventPriority::eCanWait};
	};



} // namespace se
