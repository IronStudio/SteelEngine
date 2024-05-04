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
		int someValue {183};

		se::threads::ThreadInfos infos {};
		infos.coreIndex = 3;
		infos.callback = [&someValue] () {
			printf("HAHHA : %d\n", someValue);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("OH\n");
		};
		se::threads::Thread thread {infos};
		thread.launch();

		printf("Living my life\n");
		thread.join();
		printf("End of the world\n");
	}

	catch (const se::exceptions::Exception &exception) {
		std::cerr << "ERROR : " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}