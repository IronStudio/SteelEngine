#ifndef SE_ENTRY_POINT_INCLUDED
#define SE_ENTRY_POINT_INCLUDED

#include "application.hpp"
#include "utils/assert.hpp"
#include "logging.hpp"



se::Application *createApplication();

int main(int, char **)
{
	int exitCode {EXIT_SUCCESS};
	se::Application *app {nullptr};

	try
	{
		app = createApplication();
		if (app == nullptr)
			throw std::runtime_error("Can't create application or function `createApplication` return nullptr");

		app->load();
		app->run();
	}

	catch (const std::exception &exception)
	{
		exitCode = EXIT_FAILURE;
		if (app == nullptr)
			SE_CORE_FATAL("EXCEPTION : %s", exception.what());

		else
			app->error(exception.what());
	}

	catch (const se::Assert &assert)
	{
		SE_CORE_FATAL("ASSERTION : %s", assert.getMessage().c_str());
		exitCode = EXIT_FAILURE;
	}

	if (app != nullptr)
	{
		app->unload();
		delete app;
	}

	return exitCode;
}

#endif