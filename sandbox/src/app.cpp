#include <smmintrin.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec3.hpp>
#include <se/math/vec4.hpp>
#include <se/math/matn.hpp>
#include <se/math/quaternion.hpp>

#include <se/utils/hash.hpp>
#include <se/utils/convert.hpp>
#include <se/uuid.hpp>
#include <se/eventManager.hpp>
#include <se/logging.hpp>

#include <se/layer.hpp>
#include <se/workManager.hpp>
#include <se/registry.hpp>

#include <se/input/SDL2inputManager.hpp>
#include <se/window/SDL2window.hpp>
#include <se/window/windowManager.hpp>

#include <se/application.hpp>



struct Clock
{
	Clock() :
		start {std::chrono::high_resolution_clock::now()}
	{

	}

	~Clock()
	{
		SE_DEBUG("CLOCK : %.5f ms", std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (std::chrono::high_resolution_clock::now() - start).count());
	}


	std::chrono::high_resolution_clock::time_point start;
};



class SandboxApp : public se::Application
{
public:
	SandboxApp() = default;
	~SandboxApp() override = default;

	void load() override
	{
		se::Application::load();

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error("Can't init SDL2");
	}

	void run() override
	{
		se::Logging::setLogLevel(se::LogLevel::debug);

		se::Registry registry {};
		auto entities {registry.createEntities(100)};
		auto entity24 {registry.getEntityFromUUID(entities[24]->getUUID())};
		entity24->addComponent<int> (42);
		entity24->addComponent<float> (3.14159f);
		entities[1]->addComponent<int> (12);
		entities[58]->addComponent<float> (1.6f);
		registry.sortEntities();
		SE_DEBUG("int : %u, float : %u", typeid(int).hash_code(), typeid(float).hash_code());
		SE_DEBUG("<int> : There is %d match(s)", registry.query<int> ().size());
		SE_DEBUG("<float> : There is %d match(s)", registry.query<float> ().size());
		SE_DEBUG("<int, float> : There is %d match(s)", registry.query<int, float> ().size());
		SE_DEBUG("<float, int> : There is %d match(s)", registry.query<float, int> ().size());

		se::LayerInfos layerInfos {};
		layerInfos.enabled = true;
		layerInfos.level = 1;
		layerInfos.name = "main-layer";
		auto mainLayer {se::LayerManager::addLayer(layerInfos)};


		se::WindowInfos windowInfos {};
		windowInfos.title = "SteelEngine";
		windowInfos.size = glm::ivec2(16 * 70, 9 * 70);
		windowInfos.minSize = glm::ivec2(16 * 30, 9 * 30);
		windowInfos.resizable = true;
		windowInfos.fullscreen = false;
		windowInfos.graphicsApi = se::GraphicsApi::eOpenGL;

		auto window {se::WindowManager::create<se::SDL2Window> (windowInfos)};

		windowInfos.position = glm::ivec2(100, 200);
		windowInfos.opacity = 0.7f;
		auto window2 {se::WindowManager::create<se::SDL2Window> (windowInfos)};

		se::SDL2InputManager inputManager {};

		(void)se::EventManager::addListener<se::LambdaListener> (
			mainLayer,
			window->getEventTypes().find(se::InputType::eKeyPressed)->second,
			window->getUUID(),
			[] (se::EventType type, se::Event event) -> bool {
				auto key {std::any_cast<se::Key> (event.data)};
				auto window {se::WindowManager::getFromUUID(type.linkedObject)};
				SE_DEBUG("Window : %p", window);

				if (key == se::Key::eB)
					window->toggleBorder();

				return false;
			}
		);


		while (!inputManager.shouldClose())
		{
			se::WindowManager::update();
			se::Logging::flush();

			inputManager.update();

			se::EventManager::flush();
		}
	}

	void unload() override
	{
		se::WindowManager::unload();
		SDL_Quit();
		
		se::Application::unload();
	}
};



se::Application *createApplication()
{
	return new SandboxApp();
}