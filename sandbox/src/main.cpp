#include <chrono>
#include <iostream>
#include <smmintrin.h>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec4.hpp>
#include <se/math/matn.hpp>



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
		se::Matrix<float, 4, 4> mat {
			1.f, 2.f, 3.f, 0.f,
			4.f, 5.f, 6.f, 0.f,
			7.f, 8.f, 9.f, 0.f,
			10.f, 11.f, 12.f, 1.f
		};

		std::cout << mat << std::endl;
		std::cout << mat[1][2] << std::endl;
		mat[1][2] = -0.9f;
		std::cout << mat << std::endl;
		std::cout << se::transpose(mat) << std::endl;

		std::cout << "----------\n";
		std::cout << (se::Matrix<float, 4, 4> (1.f) * se::Matrix<float, 4, 4> (1.f) == se::Matrix<float, 4, 4> (4.f)) << std::endl;
		std::cout << "----------\n";

		std::cout << se::det(se::Matn<float, 2> (1.f, 1.f, 1.f, 1.f)) << std::endl;

		mat *= -1.f;
		std::cout << mat << std::endl;

		se::Vector<float, 16> vec {mat.getInternalArray()};
		std::cout << vec << std::endl;

		se::Vector<float, 4> vec4 {1.f, 2.f, 3.f, 4.f};
		vec4 *= 2.f;
		std::cout << vec4 << std::endl;
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}