#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "se/threads/jobScheduler.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"
#include "se/types.hpp"
#include "se/window/windowManager.hpp"



namespace se {
	class Application {
		public:
			Application() = default;

			void load() {
				SE_LOGGER.setTerminalColor(se::LogColor::brightWhite);
				SE_APP_LOGGER.setTerminalColor(se::LogColor::white);
				se::threads::JobScheduler::load();
				se::window::WindowManager::load();
			}

			virtual ~Application() {
				se::window::WindowManager::unload();
				se::threads::JobScheduler::unload();
			}

			virtual void run() = 0;
	};

} // namespace se



se::Application *createApplication(const std::vector<std::string> &args);


int main(int argc, char **argv) {
	std::vector<std::string> args {};
	args.reserve(argc - 1);
	for (se::Count i {1}; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	try {
		std::unique_ptr<se::Application> app {createApplication(args)};
		if (app == nullptr)
			throw se::exceptions::BadAllocation("Can't allocate memory for the application");

		app->run();
	}

	catch (const se::exceptions::Exception &exception) {
		SE_APP_LOGGER.log({se::LogSeverity::eFatal}, exception.what());
		return EXIT_FAILURE;
	}

	catch (const std::exception &exception) {
		SE_APP_LOGGER.log({se::LogSeverity::eFatal}, exception.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
