#include <chrono>
#include <iostream>
#include <smmintrin.h>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec4.hpp>


#define PRINT_VECTOR(vector, D) for (size_t i {0}; i < D; ++i) std::cout << vector[i] << ", "; std::cout << std::endl;

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
		
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}