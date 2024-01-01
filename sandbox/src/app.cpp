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



class SandboxApp : public se::Application
{
public:
	SandboxApp() = default;
	~SandboxApp() override = default;

	void run() override
	{
		se::Logging::setLogLevel(se::LogLevel::trace);
	}
};



se::Application *createApplication()
{
	return new SandboxApp();
}