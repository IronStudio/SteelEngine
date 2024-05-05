#include "se/threads/jobScheduler.hpp"



namespace se::threads {
	JobScheduler::JobMutex::JobMutex(se::UUID uuid) :
		uuid {uuid},
		mutex {}
	{

	}

	JobScheduler::JobMutex::JobMutex(const JobMutex &jobMutex) :
		uuid {jobMutex.uuid},
		mutex {}
	{

	}

	const JobScheduler::JobMutex &JobScheduler::JobMutex::operator=(const JobMutex &jobMutex) {
		uuid = jobMutex.uuid;
		return *this;
	}



	bool JobScheduler::s_running {true};
	std::vector<se::threads::Thread> JobScheduler::s_threads;
	std::vector<se::threads::JobScheduler::JobQueue> JobScheduler::s_waitingJobs {};
	std::vector<se::threads::JobScheduler::JobMutex> JobScheduler::s_jobMutex {};
	std::mutex JobScheduler::s_waitingJobMutex {};
	std::condition_variable JobScheduler::s_newJobCV {};
	std::mutex JobScheduler::s_newJobCVMutex {};
	std::atomic_int JobScheduler::s_waitingThreadCount {0};



	void JobScheduler::load() {
		se::threads::ThreadInfos infos {};
		se::Count coreCount {std::thread::hardware_concurrency()};
		s_waitingThreadCount = coreCount;
		bool setAffinity {true};
		if (coreCount < SE_MINIMAL_CORE_COUNT) {
			coreCount = SE_MINIMAL_CORE_COUNT;
			setAffinity = false;
		}

		s_threads.reserve(coreCount);

		for (se::Count i {0}; i < coreCount; ++i) {
			infos.coreIndex = setAffinity ? i : -1;
			infos.callback = [i] () {
				s_taskThreadFunction(i);
			};

			s_threads.push_back(se::threads::Thread(infos));
			s_threads.rbegin()->launch();
		}


		s_waitingJobs.reserve((se::Count)se::threads::JobPriority::__count);
		for (se::Count i {0}; i < (se::Count)se::threads::JobPriority::__count; ++i)
			s_waitingJobs.push_back(std::queue<OpaqueJobInfos> ());

		std::cout << "Created " << coreCount << " threads" << std::endl;
	}



	void JobScheduler::unload() {
		s_running = false;
		int count {0};
		while (s_waitingThreadCount != 0);
		s_newJobCVMutex.lock();
		s_newJobCV.notify_all();
		s_newJobCVMutex.unlock();
		for (auto &thread : s_threads)
			thread.join();
	}



	void JobScheduler::waitOnJob(se::UUID job) {
		for (auto it {s_jobMutex.begin()}; it != s_jobMutex.end(); ++it) {
			if (it->uuid != job)
				continue;

			it->mutex.lock();
			s_jobMutex.erase(it);
			break;
		}
	}



	bool JobScheduler::isJobFinished(se::UUID job) {
		for (auto it {s_jobMutex.begin()}; it != s_jobMutex.end(); ++it) {
			if (it->uuid != job)
				continue;
			return false;
		}

		return true;
	}



	void JobScheduler::s_taskThreadFunction(se::Count index) {
		while (true) {
			{
				std::unique_lock lock {s_newJobCVMutex};
				--s_waitingThreadCount;
				s_newJobCV.wait(lock);
			}
			++s_waitingThreadCount;

			if (!s_running)
				return;

			OpaqueJobInfos job {};
			for (auto queue {s_waitingJobs.begin()}; queue != s_waitingJobs.end(); ++queue) {
				if (queue->empty())
					continue;
				job = queue->front();
				queue->pop();
				break;
			}

			job.callback();
			for (auto it {s_jobMutex.begin()}; it != s_jobMutex.end(); ++it) {
				if (it->uuid != job.uuid)
					continue;

				it->mutex.unlock();
			}
		}
	}

} // namespace se::threads
