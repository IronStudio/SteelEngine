#include "work.hpp"



namespace se
{
	Work::Work() :
		m_infos {},
		m_registrationTime {0}
	{

	}



	Work::Work(const se::WorkInfos &infos) :
		m_infos {infos},
		m_registrationTime {0}
	{

	}



	const se::Work &Work::operator=(const se::Work &work)
	{
		m_infos = work.m_infos;
		m_registrationTime = work.m_registrationTime;
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



	se::WorkCount Work::getRegistrationTime() const noexcept
	{
		return m_registrationTime;
	}



	void Work::increaseRegistrationTime() noexcept
	{
		++m_registrationTime;
	}



} // namespace se
