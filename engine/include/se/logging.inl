#include "logging.hpp"



namespace se
{
	se::Logger &Logging::getAppLogger()
	{
		return *s_appLogger;
	}


	
	se::Logger &Logging::getCoreLogger()
	{
		return *s_coreLogger;
	}



	void Logging::flush()
	{
		s_appLogger->flush();
		s_coreLogger->flush();
	}


	
	void Logging::setLogLevel(se::LogLevel level)
	{
		s_appLogger->setMinimalLevel(level);
		s_coreLogger->setMinimalLevel(level);
	}



} // namespace se
