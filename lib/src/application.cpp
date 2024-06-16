#include "se/application.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "se/threads/jobScheduler.hpp"
#include "se/types.hpp"
#include "se/logger.hpp"
#include "se/window/windowManager.hpp"



namespace se {

	Application::Application() :
		m_infos {}
	{
		SE_LOGGER.setTerminalColor(se::LogColor::brightWhite);
		SE_APP_LOGGER.setTerminalColor(se::LogColor::white);
		se::threads::JobScheduler::load();
		se::window::WindowManager::load();
	}


	Application::~Application() {
		se::window::WindowManager::unload();
		se::threads::JobScheduler::unload();
	}


} // namespace se
