#include <iostream>

#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>

using namespace se::literals;


int main(int, char**) {
	
	se::memory::PoolAllocator<int> allocator {10};
	auto ptr {allocator.allocate(1)};
	auto array {allocator.allocate(2)};
	auto ptr2 {allocator.allocate(1)};
	std::cout << "ptr : " << ptr << std::endl;
	std::cout << "array : " << array << std::endl;
	std::cout << "ptr2 : " << ptr2 << std::endl;

	array = allocator.reallocate(array, 2);
	std::cout << "array : " << array << std::endl;

	

	return 0;
}