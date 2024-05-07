#pragma once

#include <functional>
#include <thread>

#include "se/concepts.hpp"



namespace se::threads {
	using Callback = std::function<void()>;


	struct ThreadInfos {
		se::threads::Callback callback;
		int coreIndex;
		const int __machineCoreCount;

		ThreadInfos();
		ThreadInfos(const se::threads::ThreadInfos &infos);
		const se::threads::ThreadInfos &operator=(const se::threads::ThreadInfos &infos);
	};



#ifdef SE_LINUX

	class PthreadConfigurer {
		public:
			static void configure(const se::threads::ThreadInfos &infos);
	};

	using ThreadConfigurer = PthreadConfigurer;

#endif


	class NoneThreadConfigurer {
		public:
			inline static void configure(const se::threads::ThreadInfos &infos) {};
	};


#ifndef SE_LINUX
	using ThreadConfigurer = se::threads::NoneThreadConfigurer;
#endif



	class Thread {
		public:
			Thread();
			~Thread();

			Thread(const se::threads::ThreadInfos &infos);

			Thread(const se::threads::Thread &thread);
			const se::threads::Thread &operator=(const se::threads::Thread &thread);

			Thread(se::threads::Thread &&thread) noexcept;
			const se::threads::Thread &operator=(se::threads::Thread &&thread) noexcept;


			void launch();
			void detach();
			void join();

		private:
			std::thread m_thread;
			se::threads::ThreadInfos m_infos;
			bool m_launched;
	};
} // namespace se::threads
