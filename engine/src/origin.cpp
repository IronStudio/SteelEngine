#include "essentials/map.hpp"
#include "origin.hpp"



namespace se
{
	const se::UTF8String &convertOriginToString(se::Origin origin)
	{
		static se::Map<se::Origin, se::UTF8String> originStringMap {
			{se::Origin::eEngine, se::UTF8String("SteelEngine")},
			{se::Origin::eApp, se::UTF8String("Application")},
			{se::Origin::eUnknown, se::UTF8String("Unknown")}
		};

		return originStringMap[origin];
	}



} // namespace se

