#pragma once

#include <chrono>
#include <mutex>
#include <ostream>
#include <string>

#include "core.hpp"



namespace se
{
	enum class LogLevel
	{
		trace,
		debug,
		info,
		warning,
		error,
		fatal
	};

	class SE_CORE Logger final
	{
		public:
			Logger();
			~Logger();

			inline void setName(const std::string &name);
			inline void setStream(std::ostream *stream);
			inline void setMinimalLevel(se::LogLevel level);
			inline void flush();

			inline void trace(const std::string &format, ...);
			inline void debug(const std::string &format, ...);
			inline void info(const std::string &format, ...);
			inline void warning(const std::string &format, ...);
			inline void error(const std::string &format, ...);
			inline void fatal(const std::string &format, ...);

			void log(se::LogLevel level, const std::string &format, va_list args) SE_THREAD_SAFE;


		private:
			static std::chrono::steady_clock::time_point s_start;
			static std::mutex s_mutex;
			
			std::string m_name;
			std::ostream *m_stream;
			se::LogLevel m_minimalLevel;
			
	};



} // namespace se



#include "logger.inl"