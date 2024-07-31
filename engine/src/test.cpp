#include "test.hpp"

#include <iostream>


namespace test {
	void sayHello() {
		#ifdef SE_WINDOWS
			std::cout << "Hello from Windows SteelEngine !" << std::endl;
		#elifdef SE_LINUX
			std::cout << "Hello from Linux SteelEngine !" << std::endl;
		#elifdef SE_APPLE
			std::cout << "Hello from Apple SteelEngine !" << std::endl;
		#endif
	}
}