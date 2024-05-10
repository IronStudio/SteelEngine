#pragma once

#include <functional>

#include "se/core.hpp"
#include "se/threads/jobInfos.hpp"



namespace se::threads {
	template <typename T>
	class SE_CORE Job final {
		public:
			Job();
			~Job();

			Job(const se::threads::JobInfos<T> &infos);

			Job(const se::threads::Job<T> &job);
			const se::threads::Job<T> &operator=(const se::threads::Job<T> &job);

			void start();
			void join();
			T get();

		private:
			se::threads::JobInfos<T> m_infos;
			T m_result;
	};



} // namespace se::threads



#include "se/threads/job.inl"