#include "logger.hpp"

#include <cstdarg>
#include <stdexcept>



namespace se
{
	void Logger::setName(const std::string &name)
	{
		m_name = name;
	}



	void Logger::setStream(std::ostream *stream)
	{
		if (stream == nullptr)
			throw std::invalid_argument("Can't set logger stream to nullptr");
		m_stream = stream;
	}



	void Logger::setMinimalLevel(se::LogLevel level)
	{
		m_minimalLevel = level;
	}



	void Logger::flush()
	{
		std::lock_guard<std::mutex> _ {s_mutex};
		m_stream->flush();
	}



	void Logger::trace(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::trace, format, v);
		va_end(v);
	}



	void Logger::debug(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::debug, format, v);
		va_end(v);
	}



	void Logger::info(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::info, format, v);
		va_end(v);
	}



	void Logger::warning(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::warning, format, v);
		va_end(v);
	}



	void Logger::error(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::error, format, v);
		va_end(v);
	}



	void Logger::fatal(const std::string &format, ...)
	{
		va_list v {};
		va_start(v, format);
		this->log(se::LogLevel::fatal, format, v);
		va_end(v);
	}



} // namespace se
