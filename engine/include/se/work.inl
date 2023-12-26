#include "work.hpp"



namespace se
{
	Work::Work() :
		m_infos {}
	{

	}



	Work::Work(const se::WorkInfos &infos) :
		m_infos {infos}
	{

	}



	const se::Work &Work::operator=(const se::Work &work)
	{
		m_infos = work.m_infos;
		return *this;
	}



	const se::WorkInfos &Work::getInfos() const noexcept
	{
		return m_infos;
	}



	void Work::raisePriority() noexcept
	{
		switch (m_infos.priority)
		{
			case se::WorkPriority::eLow:
				m_infos.priority = se::WorkPriority::eMedium;
				break;
			
			case se::WorkPriority::eMedium:
				m_infos.priority = se::WorkPriority::eMedium;
				break;

			default:
				break;
		}
	}



	se::Status Work::work() SE_THREAD_SAFE
	{
		return m_infos.work(m_infos);
	}



} // namespace se
