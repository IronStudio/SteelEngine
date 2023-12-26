#pragma once

#include "core.hpp"
#include "logger.hpp"



namespace se
{
	/**
	 * @brief A global static class that manages loggers life-time and usability
	*/
	class SE_CORE Logging
	{
		public:
			static void load();
			static void unload();

			inline static se::Logger &getAppLogger();
			inline static se::Logger &getCoreLogger();

			inline static void flush();
			inline static void setLogLevel(se::LogLevel level);


		private:
			static se::Logger *s_appLogger;
			static se::Logger *s_coreLogger;
	};



} // namespace se


#ifndef NDEBUG
	#define SE_CORE_TRACE(...)   se::Logging::getCoreLogger().trace(__VA_ARGS__)
	#define SE_CORE_DEBUG(...)   se::Logging::getCoreLogger().debug(__VA_ARGS__)
	#define SE_CORE_INFO(...)    se::Logging::getCoreLogger().info(__VA_ARGS__)
	#define SE_CORE_WARNING(...) se::Logging::getCoreLogger().warning(__VA_ARGS__)

	#define SE_TRACE(...)        se::Logging::getAppLogger().trace(__VA_ARGS__)
	#define SE_DEBUG(...)        se::Logging::getAppLogger().debug(__VA_ARGS__)
	#define SE_INFO(...)         se::Logging::getAppLogger().info(__VA_ARGS__)
	#define SE_WARNING(...)      se::Logging::getAppLogger().warning(__VA_ARGS__)

#else
	#define SE_CORE_TRACE(...)
	#define SE_CORE_DEBUG(...)
	#define SE_CORE_INFO(...)
	#define SE_CORE_WARNING(...)

	#define SE_TRACE(...)
	#define SE_DEBUG(...)
	#define SE_INFO(...)
	#define SE_WARNING(...)

#endif


#define SE_CORE_ERROR(...)   se::Logging::getCoreLogger().error(__VA_ARGS__)
#define SE_CORE_FATAL(...)   se::Logging::getCoreLogger().fatal(__VA_ARGS__)

#define SE_ERROR(...)        se::Logging::getAppLogger().error(__VA_ARGS__)
#define SE_FATAL(...)        se::Logging::getAppLogger().fatal(__VA_ARGS__)


#include "logging.inl"