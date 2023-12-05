#pragma once

#include "essentials/string.hpp"



namespace se
{
	enum class Origin
	{
		eEngine,
		eApp,
		eUnknown
	};

	const se::String &convertOriginToString(se::Origin origin);

} // namespace se
