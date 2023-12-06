#include <iostream>

#include <se/essentials/assertion.hpp>
#include <se/essentials/exception.hpp>



int main(int, char **)
{
	try
	{
		
	}

	catch (const se::Exception &exception)
	{
		std::cout << exception.what() << std::endl;
	}

	catch (const se::Assertion &assertion)
	{
		std::cout << assertion.what() << std::endl;
	}

	return 0;
}