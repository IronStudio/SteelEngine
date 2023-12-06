#include "assertion.hpp"



namespace se
{
	const char *Assertion::what() const noexcept
	{
		return m_msg.getCString();
	}



} // namespace se
