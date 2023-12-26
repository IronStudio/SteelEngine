#pragma once

#include <iostream>


#define START_TEST(name) std::cout << "Start test '" << name << "'\n"; {int success {0};
#define END_TEST(name) std::cout << "\tPassed " << success << " tests\n";} std::cout << "End test '" << name << "'" << std::endl;
#define TEST(condition) if (condition) {++success;} else {std::cout << "\tFailed '" #condition "'\n";}