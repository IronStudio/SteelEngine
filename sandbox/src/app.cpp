#include <smmintrin.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec3.hpp>
#include <se/math/vec4.hpp>
#include <se/math/matn.hpp>
#include <se/math/quaternion.hpp>

#include <se/utils/hash.hpp>
#include <se/utils/convert.hpp>
#include <se/uuid.hpp>
#include <se/logging.hpp>
#include <se/event/eventManager.hpp>

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



class LambdaListener : public se::Listener
{
	public:
		LambdaListener(const se::ListenerInfos &infos, const std::function<se::Status(se::EventType, se::Event)> &callback) :
			se::Listener(infos),
			m_callback {callback}
		{

		}

		se::Status onMount() override {return se::Status::eSuccess;}
		void onUnmount() override {}


		se::Status onProcess(se::EventType eventType, se::Event event) override
		{
			return m_callback(eventType, event);
		}


	private:
		std::function<se::Status(se::EventType, se::Event)> m_callback;
};



class SandboxApp : public se::Application
{
public:
	SandboxApp() = default;
	~SandboxApp() override = default;

	void run() override
	{
		se::Logging::setLogLevel(se::LogLevel::trace);

		se::EventType type1 {};
		type1.name = "type1";
		type1.uuid = se::EventManager::addType(type1);
		se::EventType type2 {};
		type2.name = "type2";
		type2.uuid = se::EventManager::addType(type2);

		se::ListenerInfos listener1 {};
		listener1.name = "listener1";
		listener1.type = type1.uuid;
		listener1.uuid = se::EventManager::addListener<LambdaListener> (listener1, [] (se::EventType, se::Event event) -> se::Status {
			auto data = *(std::string*)event.data.get();
			SE_DEBUG("Listener1 : %s", data.c_str());
			return se::Status::eSuccess;
		});

		se::Event event1 {};
		event1.data = std::make_shared<std::string> ("event1");
		event1.priority = se::EventPriority::eBlocking;
		event1.type = type1.uuid;
		se::EventManager::notify(event1);

		se::Logging::flush();


		se::ListenerInfos listener2 {};
		listener2.name = "listener2";
		listener2.type = type1.uuid;
		listener2.uuid = se::EventManager::addListener<LambdaListener> (listener2, [] (se::EventType, se::Event event) -> se::Status {
			auto data = *(std::string*)event.data.get();
			SE_DEBUG("Listener2 : %s", data.c_str());
			return se::Status::eSuccess;
		});


		se::Event event2 {};
		event2.data = std::make_shared<std::string> ("event2");
		event2.priority = se::EventPriority::eCanWait;
		event2.type = type1.uuid;
		se::EventManager::notify(event2);

		std::this_thread::sleep_for(std::chrono::seconds(1));

		se::EventManager::flush();
	}
};



se::Application *createApplication()
{
	return new SandboxApp();
}