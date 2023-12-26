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
#include <se/logging.hpp>

#include <se/layer.hpp>
#include <se/workManager.hpp>

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

	void run() override
	{
		se::Logging::setLogLevel(se::LogLevel::debug);

		se::LayerInfos layer1Infos {};
		layer1Infos.enabled = true;
		layer1Infos.level = 0;
		layer1Infos.name = "layer1";
		auto layer1uuid = se::LayerManager::addLayer(layer1Infos);

		se::LayerInfos layer2Infos {};
		layer2Infos.enabled = true;
		layer2Infos.level = 1;
		layer2Infos.name = "layer2";
		auto layer2uuid = se::LayerManager::addLayer(layer2Infos);

		se::LayerManager::forEachEnabledLayer([](const se::LayerInfos &info) -> bool {
			static int count {0};
			SE_DEBUG("Layer %s : level %d, count : %d", info.name.c_str(), info.level, count++);
			return false;
		});

		se::EventType type1 {};
		type1.linkedObject = 0;
		auto type1uuid = se::EventManager::addType(type1);
		se::EventType type2 {};
		type2.linkedObject = 0;
		auto type2uuid = se::EventManager::addType(type2);

		auto listener1 = se::EventManager::addListener<LambdaListener> (layer1uuid, type1uuid, 0, [](se::EventType type, se::Event event) {
			SE_INFO("Called listener 1 : %s", std::any_cast<std::string> (event.data).c_str());
		});

		se::WorkInfos workInfos1 {};
		workInfos1.data = std::string("Hi");
		workInfos1.priority = se::WorkPriority::eHigh;
		workInfos1.work = [&](const se::WorkInfos &infos) {
			SE_INFO("Work : %s", std::any_cast<std::string> (infos.data).c_str());
			se::Logging::flush();
			return se::Status::eSuccess;
		};
		se::WorkManager::addWork(workInfos1);

		se::WorkInfos workInfos2 {};
		workInfos2.data = std::string("Hello");
		workInfos2.priority = se::WorkPriority::eHigh;
		workInfos2.work = [&](const se::WorkInfos &infos) {
			SE_INFO("What about that ? : %s", std::any_cast<std::string> (infos.data).c_str());
			se::Logging::flush();
			return se::Status::eSuccess;
		};
		se::WorkManager::addWork(workInfos2);

		se::Event event1 {};
		event1.lifeExpectancy = -1;
		event1.data = std::string("This is event 1");
		event1.priority = se::EventPriority::eNow;
		event1.type = type1uuid;

		se::Event event2 {};
		event2.lifeExpectancy = -1;
		event2.data = std::string("This is event 2");
		event2.priority = se::EventPriority::eCanWait;
		event2.type = type1uuid;

		se::EventManager::notify(event2);
		se::EventManager::notify(event1);

		auto listener2 = se::EventManager::addListener<LambdaListener> (layer2uuid, type1uuid, 0, [](se::EventType type, se::Event event) {
			SE_CORE_ERROR("Called listener 2 : %s", std::any_cast<std::string> (event.data).c_str());
		});

		se::LayerManager::moveLayerUp(layer2uuid);

		se::LayerManager::forEachEnabledLayer([](const se::LayerInfos &info) -> bool {
			static int count {0};
			SE_DEBUG("Layer2 %s : level %d, count : %d", info.name.c_str(), info.level, count++);
			return false;
		});

		SE_TRACE("I'm waiting");
		se::Logging::flush();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		SE_TRACE("Waiting over");

		se::EventManager::flush();
	}
};



se::Application *createApplication()
{
	return new SandboxApp();
}