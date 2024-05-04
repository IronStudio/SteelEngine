#include <iostream>
#include <thread>

#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>
#include <se/threads/work.hpp>


using namespace se::literals;


int main(int, char**) {
	try {
		se::threads::WorkInfos<int> infos {};
		infos.priority = se::threads::WorkPriority::eVeryHigh;
		infos.callback = [] () -> int {
			return 42;
		};

		se::threads::Work<int> work {infos};
		work.start();

		printf("I'm living during this time\n");

		work.join();
		printf("This is the work result : %d\n", work.get());
	}

	catch (const se::exceptions::Exception &exception) {
		std::cerr << "ERROR : " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}