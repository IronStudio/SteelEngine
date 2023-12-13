#include "application.hpp"
#include "essentials/assertion.hpp"
#include "essentials/exception.hpp"
#include "logger.hpp"



int main(int, char **)
{
	int returnCode {EXIT_SUCCESS};
	se::Application *app {nullptr};

	try
	{
		app = se::createApplication();
		if (app == nullptr)
			throw SE_AppRuntimeError("Can't create application");
			
		app->onLoad();
	}

	catch (const se::Exception &exception)
	{
		switch (exception.getOrigin())
		{
			case se::Origin::eEngine:
				SE_CoreLogger << se::LogStart() << exception.what() << se::endLog;
				break;
			case se::Origin::eApp:
				SE_CoreLogger << se::LogStart() << exception.what() << se::endLog;
				break;
			case se::Origin::eUnknown:
				SE_CoreLogger << se::LogStart() << exception.what() << se::endLog;
				break;
		}

		if (app != nullptr)
			app->onError();

		returnCode = EXIT_FAILURE;
	}

	catch (const se::Assertion &assertion)
	{
		switch (assertion.getOrigin())
		{
			case se::Origin::eEngine:
				SE_CoreLogger << se::LogStart() << assertion.what() << se::endLog;
				break;
			case se::Origin::eApp:
				SE_CoreLogger << se::LogStart() << assertion.what() << se::endLog;
				break;
			case se::Origin::eUnknown:
				SE_CoreLogger << se::LogStart() << assertion.what() << se::endLog;
				break;
		}

		returnCode = EXIT_FAILURE;
	}

	if (app != nullptr)
	{
		app->onUnload();
		delete app;
	}

	return returnCode;
}