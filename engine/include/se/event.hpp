#pragma once

#include <any>

#include "time.hpp"
#include "uuid.hpp"



namespace se
{
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
	};



} // namespace se
