#define SE_USE_MULTITHREADING
#include "workManager.hpp"

#include "logging.hpp"



namespace se
{
	std::mutex WorkManager::s_workMutex {};
	bool WorkManager::s_shutdown {false};
	std::map<se::WorkPriority, std::list<se::Work*>> WorkManager::s_works {};
	se::WorkCount WorkManager::s_worksLaunchedCount {0};
	std::vector<std::thread> WorkManager::s_threads {};
	std::vector<bool> WorkManager::s_waitingThreads {};
	std::vector<se::Work*> WorkManager::s_threadWork {};
	std::thread WorkManager::s_ownThread {};



	void WorkManager::load()
	{
		const auto coreCount {std::thread::hardware_concurrency()};
		s_workMutex.lock();
		s_threads.resize(coreCount);
		s_waitingThreads.resize(coreCount);
		s_threadWork.resize(coreCount);
		s_workMutex.unlock();

		for (auto thread {s_waitingThreads.begin()}; thread != s_waitingThreads.end(); ++thread)
			*thread = true;

		for (se::Length i {0}; i < s_threads.size(); ++i)
			s_threads[i] = std::thread(se::WorkManager::s_threadLoop, i);

		s_ownThread = std::thread(se::WorkManager::s_mainloop);
	}



	void WorkManager::unload()
	{
		s_shutdown = true;
		s_ownThread.join();

		for (se::Uint8 priority {(se::Uint8)se::WorkPriority::eLow}; priority < (se::Uint8)se::WorkPriority::eHigh; ++priority)
		{
			for (const auto &work : s_works[(se::WorkPriority)priority])
				delete work;
		}
	}



	void WorkManager::addWork(const se::WorkInfos &work) SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {s_workMutex};
		s_works[work.priority].push_back(new se::Work(work));
	}



	void WorkManager::s_mainloop()
	{
		while (!s_shutdown)
		{
			bool isOneThreadAvailable {false};
			for (const auto &thread : s_waitingThreads)
			{
				if (thread)
				{
					isOneThreadAvailable = true;
					break;
				}
			}

			if (!isOneThreadAvailable)
				continue;

			for (se::Uint8 priority {(se::Uint8)se::WorkPriority::eLow}; priority <= (se::Uint8)se::WorkPriority::eHigh; ++priority)
			{
				std::lock_guard<std::mutex> _ {s_workMutex};
				if (s_works[(se::WorkPriority)priority].empty())
					continue;

				s_sendWorkToFirstFreeThread(*s_works[(se::WorkPriority)priority].begin());
				s_works[(se::WorkPriority)priority].pop_front();
				break;
			}


			if ((s_worksLaunchedCount % 20) == 0)
			{
				for (se::Uint8 priority {(se::Uint8)se::WorkPriority::eLow}; priority < (se::Uint8)se::WorkPriority::eHigh; ++priority)
				{
					for (auto &work : s_works[(se::WorkPriority)priority])
					{
						work->raisePriority();
						s_works[(se::WorkPriority)(priority + 1)].push_back(work);
						s_works[(se::WorkPriority)priority].remove(work);
					}
				}
			}
		}


		for (auto &thread : s_threads)
			thread.join();
	}



	void WorkManager::s_threadLoop(se::Uint8 index)
	{
		SE_CORE_TRACE("Enter new work thread %d", index);

		while (!s_shutdown)
		{
			if (s_waitingThreads[index])
				continue;

			s_threadWork[index]->work();
			delete s_threadWork[index];
			s_waitingThreads[index] = true;
		}

		SE_CORE_TRACE("Leave work thread %d", index);
	}



	void WorkManager::s_sendWorkToFirstFreeThread(se::Work *work)
	{
		++s_worksLaunchedCount;
		for (se::Uint8 i {0}; i < s_threads.size(); ++i)
		{
			if (s_waitingThreads[i])
			{
				s_threadWork[i] = work;
				s_waitingThreads[i] = false;
				return;
			}
		}
	}



} // namespace se
