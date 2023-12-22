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
#include <se/eventManager.hpp>



struct Clock
{
	Clock() :
		start {std::chrono::high_resolution_clock::now()}
	{

	}

	~Clock()
	{
		std::cout << "CLOCK : " << std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (std::chrono::high_resolution_clock::now() - start).count() << " ms" << std::endl;
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


int main(int, char **)
{
	try
	{
		se::EventType type1 {};
		type1.linkedObject = 0;
		auto type1uuid = se::EventManager::addType(type1);
		se::EventType type2 {};
		type2.linkedObject = 0;
		auto type2uuid = se::EventManager::addType(type2);

		auto listener1 = se::EventManager::addListener<LambdaListener> (type1uuid, 0, [&](se::EventType type, se::Event event) {
			std::cout << "Called listener 1 : " << std::any_cast<std::string> (event.data) << std::endl;
		});
		
		se::Event event1 {};
		event1.lifeExpectancy = -1;
		event1.data = std::string("This is event 1");
		event1.priority = se::EventPriority::eBlocking;
		event1.type = type1uuid;

		se::Event event2 {};
		event2.lifeExpectancy = -1;
		event2.data = std::string("This is event 2");
		event2.priority = se::EventPriority::eCanWait;
		event2.type = type1uuid;

		se::EventManager::notify(event2);
		se::EventManager::notify(event1);

		auto listener2 = se::EventManager::addListener<LambdaListener> (type1uuid, 0, [&](se::EventType type, se::Event event) {
			std::cout << "Called listener 2 : "  << std::any_cast<std::string> (event.data) << std::endl;
		});

		std::cout << "I'm waiting" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Waiting over" << std::endl;

		se::EventManager::updateEvents();
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	se::EventManager::unload();

	return 0;
}