#include "logger.hpp"

#include <cstdlib>
#include <iostream>
#include <map>



namespace se
{
	std::chrono::steady_clock::time_point Logger::s_start {std::chrono::steady_clock::now()};



	Logger::Logger() :
		m_mutex {},
		m_name {"LOG"},
		m_stream {&std::cout}
	{

	}



	Logger::~Logger()
	{

	}



	void Logger::log(se::LogLevel level, const std::string &format, va_list args)
	{
		static std::map<se::LogLevel, std::string> levelToString {
			{se::LogLevel::trace, "\033[90mtrace\033[0m"},
			{se::LogLevel::debug, "\033[37mdebug\033[0m"},
			{se::LogLevel::info, "\033[92minfo\033[0m"},
			{se::LogLevel::warning, "\033[93mwarning\033[0m"},
			{se::LogLevel::error, "\033[91merror\033[0m"},
			{se::LogLevel::fatal, "\033[31mfatal\033[0m"}
		};

		int logSize {vsnprintf(nullptr, 0, format.c_str(), args)};
		char *output {new char[logSize + 1]};
		(void)vsnprintf(output, logSize + 1, format.c_str(), args);

		m_mutex.lock();
		*m_stream << m_name << " [" << levelToString[level] << "] ("
			<< std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - s_start)
			<< ") \033[35m>\033[0m " << output << "\n";
		m_mutex.unlock();
	}



} // namespace se
