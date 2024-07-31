#include "se/engine.hpp"

#include <iostream>



namespace se {
	Engine::Engine() {
		std::cout << "Start SteelEngine" << std::endl;
	}


	Engine::~Engine() {
		std::cout << "Shutdown SteelEngine" << std::endl;
	}


} // namespace se
