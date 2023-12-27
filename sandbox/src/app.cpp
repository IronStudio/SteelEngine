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

#include <se/window/SDL2window.hpp>

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


class LambdaListener : public se::EventListener
{
	public:
		LambdaListener(se::UUID eventType, se::UUID linkedObject, const std::function<void(se::EventType, se::Event)> &callback) :
			se::EventListener(eventType, linkedObject),
			m_callback {callback}
		{

		}

		virtual ~LambdaListener() override = default;

		virtual void process(se::EventType type, se::Event event) SE_THREAD_SAFE override
		{
			m_callback(type, event);
		}


	private:
		std::function<void(se::EventType, se::Event)> m_callback;
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


		se::EventManager::flush();


		se::WindowInfos windowInfos {};
		windowInfos.title = "SteelEngine";
		windowInfos.size = glm::ivec2(16 * 70, 9 * 70);
		windowInfos.minSize = glm::ivec2(16 * 30, 9 * 30);
		windowInfos.resizable = true;
		windowInfos.fullscreen = false;
		windowInfos.graphicsApi = se::GraphicsApi::eOpenGL;
		se::SDL2Window window {windowInfos};

		while (true)
		{
			window.updateInfos();
			se::Logging::flush();

			SDL_Event event {};
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					return;

				if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)
					window.toggleFullscreen();

				if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_B)
					window.toggleBorder();
			}
		}
	}

	void unload() override
	{
		SDL_Quit();
		
		se::Application::unload();
	}
};



se::Application *createApplication()
{
	return new SandboxApp();
}