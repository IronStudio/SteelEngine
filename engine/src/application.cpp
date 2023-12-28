#define SE_ENTRY_POINT_INCLUDED
#include "application.hpp"

#include "eventManager.hpp"
#include "logging.hpp"
#include "window/windowManager.hpp"
#include "workManager.hpp"



namespace se
{
	void Application::load()
	{
		se::Logging::load();
		se::WorkManager::load();
		se::WindowManager::load();
	}



	void Application::unload()
	{
		se::EventManager::unload();
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
