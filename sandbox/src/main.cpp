#include <iostream>

#include <se/essentials/assertion.hpp>
#include <se/essentials/exception.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/memory/stackAllocator.hpp>



int main(int, char **)
{
	try
	{
		se::PoolAllocator<int> allocator {3};
		auto var1 = allocator.allocate();
		auto var2 = allocator.allocate();
		auto var3 = allocator.allocate();

		allocator.free(var3);

		std::cout << allocator.getUsage() << std::endl;

		auto var4 = allocator.allocate();
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