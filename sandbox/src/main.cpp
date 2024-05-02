#include <iostream>
#include <thread>

#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>


using namespace se::literals;


int main(int, char**) {
	try {
		se::threads::Thread thread {};
		thread.bind([] (se::threads::SerializedArgs args) {
			int age = std::get<int> (se::threads::deserialize<int> (args));
			printf("HAHA age is %d\n", age);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("Ouf, age is %d\n", age);
		});

		thread.launch(se::threads::SerializedArgs(int(18)));

		printf("I'm living my life\n");
		printf("It's sooooo nice...\n");

		thread.waitOn();
	}

	catch (const se::exceptions::Exception &exception) {
		std::cerr << "ERROR : " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}