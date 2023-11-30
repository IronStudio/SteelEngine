#include "essentials/exception.hpp"
#include "essentials/map.hpp"



namespace se::essentials
{
	se::essentials::Map<se::essentials::ExceptionOrigin, se::essentials::String> originStringMap {
		{se::essentials::ExceptionOrigin::eEngine, "SteelEngine"},
		{se::essentials::ExceptionOrigin::eApp, "Application"},
		{se::essentials::ExceptionOrigin::eUnknown, "Unknown"}
	};



	RuntimeError::RuntimeError(
		const se::essentials::String &file,
		int line,
		const se::essentials::String &func,
		const se::essentials::String &message,
		se::essentials::ExceptionOrigin origin
	) : 
		m_string {}
	{
		m_string = originStringMap[origin] + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line) + " > " + message;
	}



	const se::essentials::String &RuntimeError::what() const noexcept
	{
		return m_string;
	}



	InvalidArgumentError::InvalidArgumentError(
		const se::essentials::String &file,
		int line,
		const se::essentials::String &func,
		const se::essentials::String &argument,
		const se::essentials::String &message,
		se::essentials::ExceptionOrigin origin
	) : 
		m_string {}
	{
		m_string = originStringMap[origin] + " : in function '" + func + "', file '" + file + "', line " + std::to_string(line)
			+ " > argument '" + argument + "' is invalid : " + message;
	}



	const se::essentials::String &InvalidArgumentError::what() const noexcept
	{
		return m_string;
	}



} // namespace se::essentials
