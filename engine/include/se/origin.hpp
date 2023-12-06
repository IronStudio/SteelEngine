#pragma once

#include "core.hpp"
#include "essentials/string.hpp"



namespace se
{
	enum class Origin
	{
		eEngine,
		eApp,
		eUnknown
	};

	SE_CORE const se::UTF8String &convertOriginToString(se::Origin origin);

} // namespace se
