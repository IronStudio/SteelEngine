#include <iostream>

#include <se/engine.hpp>
#include <se/utils/string.hpp>


int main() {
	se::Engine engine {};

	std::cout << "Wide char size : " << sizeof(wchar_t) << std::endl;
	se::String<se::StringEncoding::eUtf8> string1 {"Hello World ! éhéhé"};
	std::cout << string1.getData() << std::endl;
	se::String<se::StringEncoding::eUtf16> string2 {u"I'm using UTF16 éhéhé !Ḁ𝓛"};
	std::cout << string2.useEncoding<se::StringEncoding::eUtf8> ().getData() << std::endl;
	se::String<se::StringEncoding::eUtf8> string3 {"I'm using UTF8 to UTF16 éhéhé !Ḁ𝓛"};
	std::cout << string3.useEncoding<se::StringEncoding::eUtf16> ().useEncoding<se::StringEncoding::eUtf8> ().getData() << std::endl;

	return 0;
}