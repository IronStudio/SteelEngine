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



	struct WorkInfos {
		se::threads::WorkPriority priority {se::threads::WorkPriority::eNormal};
	};



	class Work final {
		public:
			Work();
			~Work();

		private:
			std::function<void()> m_callback;
	};



} // namespace se::threads
