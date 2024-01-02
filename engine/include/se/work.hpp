#pragma once

#ifndef SE_USE_MULTITHREADING
	#error The multithreading part of SteelEngine isn't ready yet. Please avoid using it
#endif

#include <any>
#include <functional>

#include "status.hpp"
#include "types.hpp"
#include "uuid.hpp"


#define SE_THREAD_SAFE


namespace se
{
	using WorkCount = se::Uint32;

	enum class WorkPriority : se::Int8
	{
		eLow = 1,
		eMedium,
		eHigh
	};


	struct WorkInfos
	{
		std::function<se::Status(se::WorkInfos infos) SE_THREAD_SAFE> work;
		std::any data;
		se::WorkPriority priority;
	};


	/**
	 * @brief A class that handles a work (ie something to execute on another thread)
	*/
	class Work
	{
		public:
			inline Work();
			inline Work(const se::WorkInfos &infos);

			inline const se::Work &operator=(const se::Work &work);

			inline const se::WorkInfos &getInfos() const noexcept;
			inline void raisePriority() noexcept;
			inline se::Status work() SE_THREAD_SAFE;


		private:
			se::WorkInfos m_infos;
	};



} // namespace se



#include "work.inl"