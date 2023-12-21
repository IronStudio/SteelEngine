#include "uuid.hpp"



namespace se
{
	template <typename T>
	se::UUID UUIDManager::generate () SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_mutex};
		++m_lastGeneratedUnnamedUUID;
		m_UUIDToTypes[m_lastGeneratedUnnamedUUID] = typeid(T);
		return m_lastGeneratedUnnamedUUID;
	}



	template <typename T>
	se::UUID UUIDManager::generate (const std::string &name) SE_THREAD_SAFE
	{
		se::UUID std::hash;
		std::lock_guard<std::mutex> _ {m_mutex};
	}



} // namespace se
