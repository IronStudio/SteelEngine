#include <iostream>
#include <thread>

#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>
#include <se/threads/job.hpp>
#include <se/threads/jobScheduler.hpp>


using namespace se::literals;


int main(int, char**) {
	se::threads::JobScheduler::load();

	try {
		se::threads::JobInfos<int> infos {};
		infos.priority = se::threads::JobPriority::eVeryHigh;
		infos.callback = [] () -> int {
			return 42;
		};

		se::threads::Job<int> job {infos};
		job.start();

		printf("I'm living during this time\n");

		job.join();
		printf("This is the Job result : %d\n", job.get());
	}

	catch (const se::exceptions::Exception &exception) {
		std::cerr << "ERROR : " << exception.what() << std::endl;
		se::threads::JobScheduler::unload();
		return EXIT_FAILURE;
	}

	se::threads::JobScheduler::unload();
	return EXIT_SUCCESS;
}