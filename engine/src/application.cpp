#define SE_ENTRY_POINT_INCLUDED
#include "application.hpp"

#include "logging.hpp"
#include "workManager.hpp"



namespace se
{
	void Application::load()
	{
		se::Logging::load();
		se::WorkManager::load();
	}



	void Application::unload()
	{
		se::WorkManager::unload();
		se::Logging::unload();
	}



	void Application::error(const std::string &msg)
	{
		SE_FATAL(msg);
	}



	void Application::run()
	{

	}



} // namespace se
