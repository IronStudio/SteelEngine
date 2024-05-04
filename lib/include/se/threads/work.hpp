#pragma once

#include <functional>



namespace se::threads {
	enum WorkPriority {
		eVeryHigh,
		eHigh,
		eNormal,
		eLow,
		eVeryLow
	};



	template <typename T>
	struct WorkInfos {
		se::threads::WorkPriority priority {se::threads::WorkPriority::eNormal};
		std::function<T()> callback;
	};



	template <typename T>
	class Work final {
		public:
			Work();
			~Work();

			Work(const se::threads::WorkInfos<T> &infos);

			void start();
			void join();
			T get();

		private:
			se::threads::WorkInfos<T> m_infos;
			T m_result;
	};



} // namespace se::threads
