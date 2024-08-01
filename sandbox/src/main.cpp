#include <iostream>

#include <se/engine.hpp>
#include <se/utils/string.hpp>


int main() {
	se::Engine engine {};

	se::String<se::StringEncoding::eUtf8> str1 {"Hello "};
	se::String<se::StringEncoding::eUtf8> str2 {"World !"};
	auto str3 {str1 + str2};
	std::cout << str3.getData() << std::endl;
	str1 += str2;
	std::cout << str1.getData() << std::endl;

	return 0;
}