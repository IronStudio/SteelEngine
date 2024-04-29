#include <iostream>

#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>

using namespace se::literals;


int main(int, char**) {
	srand(time(nullptr));

	std::cout << "Hello World !" << std::endl;

	se::UUID uuid {};
	std::cout << "uuid : " << uuid << std::endl;
	se::UUID uuid2 {"123f-20ca-6d8e-eb14"};
	std::cout << "uuid2 : " << uuid2 << std::endl;

	return 0;
}