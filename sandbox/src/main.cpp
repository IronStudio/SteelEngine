#include <iostream>
#include <thread>

#include <vulkan/vulkan.h>

#define SE_APPLICATION_MAIN_FILE
#include <se/steelEngine.hpp>
#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>
#include <se/threads/job.hpp>
#include <se/threads/jobScheduler.hpp>
#include <se/ecs/scene.hpp>
#include <se/utils/version.hpp>
#include <se/renderer/vulkan/context.hpp>


using namespace se::literals;


class SandboxApp : public se::Application {
	public:
		SandboxApp() {
			this->load();
		}

		~SandboxApp() override {}

		void run() override {
			/*se::threads::JobInfos<int> infos {};
			infos.priority = se::threads::JobPriority::eVeryHigh;
			infos.callback = [] () -> int {
				return 42;
			};

			se::threads::Job<int> job {infos};
			job.start();

			printf("I'm living during this time\n");

			job.join();
			printf("This is the Job result : %d\n", job.get());*/

			se::renderer::ContextInfos contextInfos {};
			contextInfos.applicationName = "SteelEngine_sandbox";
			contextInfos.applicationVersion = "1.0.0"_v;
			contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
			se::renderer::vulkan::Context context {contextInfos};
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}