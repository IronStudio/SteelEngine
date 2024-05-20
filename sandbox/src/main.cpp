#include <iostream>
#include <thread>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#define SE_APPLICATION_MAIN_FILE
#include <se/steelEngine.hpp>
#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/engine.hpp>
#include <se/input/inputManager.hpp>
#include <se/math.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>
#include <se/threads/job.hpp>
#include <se/threads/jobScheduler.hpp>
#include <se/ecs/scene.hpp>
#include <se/utils/version.hpp>
#include <se/renderer/vulkan/context.hpp>
#include <se/window/windowManager.hpp>


using namespace se::literals;


class SandboxApp : public se::Application {
	public:
		SandboxApp() {
			this->load();
		}

		~SandboxApp() override {}

		void run() override {
			SE_LOGGER.setSeverityMask(se::LogSeverity::eError | se::LogSeverity::eFatal | se::LogSeverity::eWarning | se::LogSeverity::eInfo);

			SE_LOGGER << se::LogInfos(se::LogSeverity::eVerbose) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eWarning) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eError) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eFatal) << "Hello" << se::endLog;

			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eVerbose) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eWarning) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eError) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eFatal) << "Hello" << se::endLog;

			se::renderer::ContextInfos contextInfos {};
			contextInfos.applicationName = "SteelEngine_sandbox";
			contextInfos.applicationVersion = "1.0.0"_v;
			contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
			se::renderer::vulkan::Context context {contextInfos};


			se::window::WindowInfos windowInfos {};
			windowInfos.title = "SteelEngine_sandbox";
			windowInfos.size = {16 * 70, 9 * 70};
			windowInfos.position = {se::window::centerPosition, se::window::undefinedPosition};
			windowInfos.graphicsApi = se::renderer::GraphicsApi::eVulkan;
			se::window::Window &window {se::window::WindowManager::createWindow(windowInfos)};

			windowInfos.title = "2nd";
			se::window::Window &window2 {se::window::WindowManager::createWindow(windowInfos)};

			bool running {true};
			while (se::Engine::isRunning()) {
				se::input::InputManager::update();
				if (se::input::InputManager::isKeyDown(se::input::Key::eA))
					std::cout << "A" << std::endl;
				if (se::input::InputManager::wasKeyPressed(se::input::Key::eD))
					std::cout << "D" << std::endl;
				if (se::input::InputManager::wasKeyReleased(se::input::Key::eF))
					std::cout << "F" << std::endl;
			}
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}