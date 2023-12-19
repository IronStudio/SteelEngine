#include <chrono>
#include <iostream>
#include <smmintrin.h>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec3.hpp>
#include <se/math/vec4.hpp>
#include <se/math/matn.hpp>
#include <se/math/quaternion.hpp>



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
		std::cout << se::cross(se::Vec3f(1.f, 0.f, 0.f), se::Vec3f(0.f, 1.f, 0.f)) << std::endl;;
		se::Quaternion<float> quat {sinf(2.f) * se::Vec3f(1.f, 0.f, 0.f), cosf(2.f)};
		se::Quaternion<float> quat2 {sinf(1.f) * se::Vec3f(0.f, 1.f, 0.f), cosf(1.f)};
		std::cout << quat << " : " << quat.length() << std::endl;
		std::cout << quat2 << " : " << quat2.length() << std::endl;
		auto quat3 {quat};
		quat3 *= quat2;
		std::cout << quat3 << " : " << quat3.length() << std::endl;
		quat3.normalize();
		std::cout << quat3 << " : " << quat3.length() << std::endl;
		std::cout << (quat * quat) << " : " << (quat * quat).length() << std::endl;
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}