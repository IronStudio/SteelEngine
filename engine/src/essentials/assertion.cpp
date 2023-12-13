#include "essentials/assertion.hpp"
#include "essentials/string.hpp"




namespace se
{
	Assertion::Assertion(
		const se::UTF8String &file,
		se::Uint line,
		const se::UTF8String &func,
		const se::UTF8String &msg,
		se::Origin origin
	) :
		m_msg {},
		m_origin {origin}
	{
		m_msg = se::convertOriginToString(origin) + " ASSERTION : in file '" + file + "', line "
			+ se::intToString<se::Charset::UTF8> (line) + ", function '" + func + "' > " + msg;
	}



} // namespace se
