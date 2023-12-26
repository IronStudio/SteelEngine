#include <iostream>

#include <se/utils/assert.hpp>
#include <se/math/vec4.hpp>

#include "test.hpp"



int main(int, char **)
{
	try
	{
		#include "vector.inl"
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}