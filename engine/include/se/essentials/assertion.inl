#include "assertion.hpp"



namespace se
{
	const char *Assertion::what() const noexcept
	{
		return m_msg.getCString();
	}



	se::Origin Assertion::getOrigin() const noexcept
	{
		return m_origin;
	}



} // namespace se
