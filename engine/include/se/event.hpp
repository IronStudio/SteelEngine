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
		se::UUID type;
		std::any data;
		se::EventPriority priority {se::EventPriority::eCanWait};
		se::Frame lifeExpectancy {-1};
	};



} // namespace se
