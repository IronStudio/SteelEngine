#include "se/threads/job.hpp"

#include "se/assertion.hpp"
#include "se/threads/jobScheduler.hpp"



namespace se::threads {
	template <typename T>
	Job<T>::Job() :
		m_infos {},
		m_result {}
	{
		m_infos.__callback = [this] () {
			this->m_result = this->m_infos.callback();
		};
	}



	template <typename T>
	Job<T>::~Job() {

	}



	template <typename T>
	Job<T>::Job(const se::threads::JobInfos<T> &infos) :
		m_infos {infos},
		m_result {}
	{
		m_infos.__callback = [this] () {
			this->m_result = this->m_infos.callback();
		};
	}



	template <typename T>
	Job<T>::Job(const se::threads::Job<T> &job) :
		m_infos {job.m_infos},
		m_result {job.m_result}
	{
		m_infos.__callback = [this] () {
			this->m_result = this->m_infos.callback();
		};
	}



	template <typename T>
	const se::threads::Job<T> &Job<T>::operator=(const se::threads::Job<T> &job) {
		m_infos = job.m_infos;
		m_result = job.m_result;
		m_infos.__callback = [this] () {
			this->m_result = this->m_infos.callback();
		};
		return *this;
	}



	template <typename T>
	void Job<T>::start() {
		se::threads::JobScheduler::submitJob<T> (m_infos);
	}



	template <typename T>
	void Job<T>::join() {
		se::threads::JobScheduler::waitOnJob(m_infos.uuid);
	}



	template <typename T>
	T Job<T>::get() {
		SE_ASSERT(se::threads::JobScheduler::isJobFinished(m_infos.uuid), "Can't get result of unfinished job");
		return m_result;
	}

} // namespace se::threads
