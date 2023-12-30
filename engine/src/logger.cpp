#include "logger.hpp"

#include <cstdlib>
#include <iostream>
#include <map>

#include "types.hpp"



namespace se
{
	std::chrono::steady_clock::time_point Logger::s_start {std::chrono::steady_clock::now()};
	std::mutex Logger::s_mutex {};



	Logger::Logger() :
		m_name {"LOG"},
		m_stream {&std::cout},
		m_minimalLevel {se::LogLevel::trace}
	{

	}



	Logger::~Logger()
	{
		this->flush();
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

		if ((se::Uint8)level < (se::Uint8)m_minimalLevel)
			return;

		auto duration {std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (std::chrono::steady_clock::now() - s_start)};
		int logSize {vsnprintf(nullptr, 0, format.c_str(), args)};
		char *output {new char[logSize + 1]};
		(void)vsnprintf(output, logSize + 1, format.c_str(), args);

		{
			std::lock_guard<std::mutex> _ {s_mutex};
			*m_stream << m_name << " [" << levelToString[level] << "] ("
				<< duration.count() << "ms) \033[35m>\033[0m " << output << "\n";
		}
	}



} // namespace se
