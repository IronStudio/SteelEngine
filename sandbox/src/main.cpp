#include <iostream>

#include <se/utils/assert.hpp>



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