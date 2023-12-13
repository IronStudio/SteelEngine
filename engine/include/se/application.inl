#include <iostream>

#include "application.hpp"
#include "logger.hpp"



namespace se
{
	void Application::onLoad()
	{
		se::Logger<se::Origin::eApp>::load();
		se::Logger<se::Origin::eEngine>::load();
		se::Logger<se::Origin::eUnknown>::load();
		SE_AppLogger.setStream(&std::cout);
		SE_CoreLogger.setStream(&std::cout);
		SE_UnknownLogger.setStream(&std::cout);
	}



	void Application::onUnload() noexcept
	{
		se::Logger<se::Origin::eEngine>::unload();
		se::Logger<se::Origin::eUnknown>::unload();
		se::Logger<se::Origin::eApp>::unload();
	}



} // namespace se
