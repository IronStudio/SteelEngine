#include <chrono>
#include <iostream>
#include <smmintrin.h>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec3.hpp>
#include <se/math/vec4.hpp>
#include <se/math/matn.hpp>
#include <se/math/quaternion.hpp>

#include <se/utils/hash.hpp>
#include <se/utils/convert.hpp>
#include <se/uuid.hpp>



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


int main(int, char **)
{
	try
	{
		auto a = se::UUIDManager::generate<float> ();
		auto b = se::UUIDManager::generate<int> ("this-one-is-named");
		std::cout << se::int64ToString(a, SE_UUID_BASE) << ", " << se::UUIDManager::getType(a) << std::endl;
		std::cout << se::int64ToString(b, SE_UUID_BASE) << ", " << se::UUIDManager::getName(b) << std::endl;
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}