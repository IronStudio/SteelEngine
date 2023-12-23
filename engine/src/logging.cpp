#include "logging.hpp"



namespace se
{
	se::Logger *Logging::s_appLogger {nullptr};
	se::Logger *Logging::s_coreLogger {nullptr};



	void Logging::load()
	{
		std::ios::sync_with_stdio(false);
		
		s_appLogger = new se::Logger();
		s_coreLogger = new se::Logger();

		s_appLogger->setName("\033[1mAPP\033[0m");
		s_coreLogger->setName("\033[96mSTEEL\033[0m");
	}



	void Logging::unload()
	{
		delete s_coreLogger;
		delete s_appLogger;
	}



} // namespace se
