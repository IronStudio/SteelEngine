#include <iostream>

#include <se/engine.hpp>
#include <se/utils/iterator.hpp>
#include <se/utils/string.hpp>



int main() {
	se::Engine engine {};

	se::String str1 {"Hello "};
	se::String str2 {"World !"};
	auto str3 {str1 + str2};
	std::cout << str3.getData() << std::endl;
	str1 += str2;
	std::cout << str1.getData() << std::endl;

	for (auto it {str1.begin()}; it != str1.end(); ++it)
		std::cout << "\t" << *it << std::endl;

	return 0;
}