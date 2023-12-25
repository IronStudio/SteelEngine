#pragma once

#include <map>
#include <list>
#include <thread>
#include <vector>

#include "core.hpp"
#include "work.hpp"



namespace se
{
	class SE_CORE WorkManager
	{
		public:
			static void load();
			static void unload();

			static void addWork(const se::WorkInfos &work) SE_THREAD_SAFE;


		private:
			static void s_mainloop();
			static void s_threadLoop(se::Uint8 index);

			static void s_sendWorkToFirstFreeThread(se::Work *work);

			static std::mutex s_workMutex;
			static bool s_shutdown;
			static std::map<se::WorkPriority, std::list<se::Work*>> s_works;
			static se::WorkCount s_worksLaunchedCount;
			static std::vector<std::thread> s_threads;
			static std::vector<bool> s_waitingThreads;
			static std::vector<se::Work*> s_threadWork;
			static std::thread s_ownThread;
	};



} // namespace se
