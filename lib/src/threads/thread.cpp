#include "se/threads/thread.hpp"

#if defined(SE_LINUX) || defined(SE_APPLE)
	extern "C" {
		#include <pthread.h>
	}
#endif

#include "se/assertion.hpp"
#include "se/exceptions.hpp"



namespace se::threads {

#if defined(SE_LINUX) || defined(SE_APPLE)

	void PthreadConfigurer::configure(const se::threads::ThreadInfos &infos) {
		if (infos.coreIndex < 0)
			return;

		SE_ASSERT(infos.coreIndex < infos.__machineCoreCount, "Can't affect thread to cpu core that does not exist");

		cpu_set_t *cpuset {CPU_ALLOC(infos.__machineCoreCount)};
		CPU_ZERO_S(infos.__machineCoreCount, cpuset);
		CPU_SET_S(infos.coreIndex, infos.__machineCoreCount, cpuset);

		pthread_t currentThread {pthread_self()};
		if (pthread_setaffinity_np(currentThread, infos.__machineCoreCount, cpuset) != 0) {
			CPU_FREE(cpuset);
			throw se::exceptions::RuntimeError("Can't set affinity of the newly created pthread");
		}

		CPU_FREE(cpuset);
	}

#endif



	ThreadInfos::ThreadInfos() :
		callback {nullptr},
		coreIndex {-1},
		__machineCoreCount {(int)std::thread::hardware_concurrency()}
	{

	}



	ThreadInfos::ThreadInfos(const se::threads::ThreadInfos &infos) :
		callback {infos.callback},
		coreIndex {infos.coreIndex},
		__machineCoreCount {infos.__machineCoreCount}
	{

	}



	const se::threads::ThreadInfos &ThreadInfos::operator=(const se::threads::ThreadInfos &infos) {
		callback = infos.callback;
		coreIndex = infos.coreIndex;
		return *this;
	}





	Thread::Thread() :
		m_thread {},
		m_infos {},
		m_launched {false}
	{

	}



	Thread::~Thread() {
		
	}



	Thread::Thread(const se::threads::ThreadInfos &infos) :
		m_thread {},
		m_infos {infos},
		m_launched {false}
	{
		m_infos.callback = [infos] () {
			se::threads::ThreadConfigurer::configure(infos);
			infos.callback();
		};
	}



	Thread::Thread(const se::threads::Thread &thread) :
		m_thread {},
		m_infos {thread.m_infos},
		m_launched {false}
	{

	}



	const se::threads::Thread &Thread::operator=(const se::threads::Thread &thread)	{
		SE_ASSERT(!m_launched, "Can't copy thread into already launched thread");
		m_infos = thread.m_infos;
		return *this;
	}



	Thread::Thread(se::threads::Thread &&thread) noexcept :
		m_thread {std::move(thread.m_thread)},
		m_infos {thread.m_infos},
		m_launched {thread.m_launched}
	{
		thread.m_thread = {};
		thread.m_infos = {};
		thread.m_launched = false;
	}



	const se::threads::Thread &Thread::operator=(se::threads::Thread &&thread) noexcept {
		SE_ASSERT(!m_launched, "Can't move thread into already launched thread");
		m_thread = std::move(thread.m_thread);
		m_infos = thread.m_infos;
		m_launched = thread.m_launched;

		thread.m_thread = {};
		thread.m_infos = {};
		thread.m_launched = false;

		return *this;
	}



	void Thread::launch() {
		SE_ASSERT(!m_launched, "Can't launch thread that has already been launched");
		m_thread = std::thread(m_infos.callback);
		m_launched = true;
	}



	void Thread::detach() {
		SE_ASSERT(m_launched, "Can't detach thread that was not launched");
		m_thread.detach();
	}



	void Thread::join() {
		SE_ASSERT(m_launched, "Can't join thread that was not launched");
		m_thread.join();
	}



} // namespace se::threads
