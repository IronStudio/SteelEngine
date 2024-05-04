#pragma once

#include <functional>

#include "se/uuid.hpp"



namespace se::threads {
	enum JobPriority {
		eVeryHigh = 0,
		eHigh,
		eNormal,
		eLow,
		eVeryLow,

		__count
	};



	template <typename T>
	struct JobInfos {
		se::threads::JobPriority priority {se::threads::JobPriority::eNormal};
		std::function<T()> callback;
		se::UUID uuid;
		std::function<void()> __callback;
	};
} // namespace se::threads
