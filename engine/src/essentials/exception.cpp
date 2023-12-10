#include "essentials/exception.hpp"



namespace se
{
	RuntimeError::RuntimeError(
		const se::UTF8String &file,
		se::Uint line,
		const se::UTF8String &func,
		const se::UTF8String &message,
		se::Origin origin
	) : 
		m_string {}
	{
		m_string = se::convertOriginToString(origin) + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line) + " > " + message;
	}



	const se::UTF8String &RuntimeError::what() const noexcept
	{
		return m_string;
	}



	InvalidArgumentError::InvalidArgumentError(
		const se::UTF8String &file,
		se::Uint line,
		const se::UTF8String &func,
		const se::UTF8String &argument,
		const se::UTF8String &message,
		se::Origin origin
	) : 
		m_string {}
	{
		m_string = se::convertOriginToString(origin) + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line)
			+ " > argument '" + argument + "' is invalid : " + message;
	}



	const se::UTF8String &InvalidArgumentError::what() const noexcept
	{
		return m_string;
	}



} // namespace se
