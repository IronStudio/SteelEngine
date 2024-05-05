#pragma once

#include <atomic>
#include <condition_variable>
#include <queue>
#include <vector>

#include "se/core.hpp"
#include "se/threads/jobInfos.hpp"
#include "se/threads/thread.hpp"


#ifndef SE_MINIMAL_CORE_COUNT
	#define SE_MINIMAL_CORE_COUNT 4
#endif


namespace se::threads {
	class SE_CORE JobScheduler {
		struct OpaqueJobInfos {
			se::threads::JobPriority priority;
			se::UUID uuid;
			std::function<void()> callback;
		};

		struct JobMutex {
			se::UUID uuid;
			std::mutex mutex;

			JobMutex(se::UUID uuid);
			JobMutex(const JobMutex &jobMutex);
			const JobMutex &operator=(const JobMutex &jobMutex);
		};

		using JobQueue = std::queue<OpaqueJobInfos>;

		public:
			static void load();
			static void unload();

			template <typename T>
			static void submitJob(const se::threads::JobInfos<T> &infos);

			static void waitOnJob(se::UUID job);
			static bool isJobFinished(se::UUID job);

		private:
			static void s_taskThreadFunction(se::Count index);

			static bool s_running;
			static std::vector<se::threads::Thread> s_threads;
			static std::vector<JobQueue> s_waitingJobs;
			static std::vector<JobMutex> s_jobMutex;
			static std::mutex s_waitingJobMutex;
			static std::condition_variable s_newJobCV;
			static std::mutex s_newJobCVMutex;
			static std::atomic_int s_waitingThreadCount;
	};
} // namespace se::threads



#include "se/threads/jobScheduler.inl"