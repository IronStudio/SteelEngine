#ifdef AUHSJDHAHJDJASJKLSJKLDAJSA

#include "essentials/exception.hpp"
#include "essentials/map.hpp"



namespace se
{
	RuntimeError::RuntimeError(
		const se::String &file,
		int line,
		const se::String &func,
		const se::String &message,
		se::Origin origin
	) : 
		m_string {}
	{
		m_string = se::convertOriginToString(origin) + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line) + " > " + message;
	}



	const se::String &RuntimeError::what() const noexcept
	{
		return m_string;
	}



	InvalidArgumentError::InvalidArgumentError(
		const se::String &file,
		int line,
		const se::String &func,
		const se::String &argument,
		const se::String &message,
		se::Origin origin
	) : 
		m_string {}
	{
		m_string = se::convertOriginToString(origin) + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line)
			+ " > argument '" + argument + "' is invalid : " + message;
	}



	const se::String &InvalidArgumentError::what() const noexcept
	{
		return m_string;
	}



} // namespace se


#endif