#pragma once

#include <functional>



namespace se::threads {
	enum JobPriority {
		eVeryHigh,
		eHigh,
		eNormal,
		eLow,
		eVeryLow
	};



	template <typename T>
	struct JobInfos {
		se::threads::JobPriority priority {se::threads::JobPriority::eNormal};
		std::function<T()> callback;
	};



	template <typename T>
	class Job final {
		public:
			Job();
			~Job();

			Job(const se::threads::JobInfos<T> &infos);

			void start();
			void join();
			T get();

		private:
			se::threads::JobInfos<T> m_infos;
			T m_result;
	};



} // namespace se::threads
