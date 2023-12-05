#ifdef aildjsaidieijlaksjdkadsa

#include "origin.hpp"



namespace se
{
	const se::String &convertOriginToString(se::Origin origin)
	{
		se::Map<se::Origin, se::String> originStringMap {
			{se::Origin::eEngine, "SteelEngine"},
			{se::Origin::eApp, "Application"},
			{se::Origin::eUnknown, "Unknown"}
		};

		return originStringMap[origin];
	}



} // namespace se


#endif