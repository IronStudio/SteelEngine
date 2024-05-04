#include "se/threads/jobScheduler.hpp"



namespace se::threads {
	template <typename T>
	void JobScheduler::submitJob(const se::threads::JobInfos<T> &infos) {
		OpaqueJobInfos job {};
		job.priority = infos.priority;
		job.uuid = infos.uuid;
		job.callback = infos.__callback;
		s_waitingJobMutex.lock();
		s_waitingJobs[(se::Count)infos.priority].push(job);
		s_jobMutex.push_back(JobMutex(infos.uuid));
		s_jobMutex.rbegin()->mutex.lock();
		s_waitingJobMutex.unlock();

		s_newJobCVMutex.lock();
		s_newJobCV.notify_one();
		s_newJobCVMutex.unlock();
	}

} // namespace se::threads
