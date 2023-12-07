#pragma once

#include "core.hpp"



namespace se
{
	enum class Origin
	{
		eEngine,
		eApp,
		eUnknown
	};
} // namespace se


#include "essentials/string.hpp"


namespace se
{
	SE_CORE const se::UTF8String &convertOriginToString(se::Origin origin);

} // namespace se
