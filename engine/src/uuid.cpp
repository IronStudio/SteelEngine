#include "uuid.hpp"

#include "utils/convert.hpp"



namespace se
{
	std::mutex UUIDManager::m_mutex {};
	std::map<std::string, se::UUID> UUIDManager::m_nameToUUIDs {};
	std::map<se::UUID, se::Uint64> UUIDManager::m_UUIDToTypes {};
	se::UUID UUIDManager::m_lastGeneratedUnnamedUUID {0};



	se::UUID UUIDManager::getUUID(const std::string &name) SE_THREAD_SAFE
	{
		auto it {m_nameToUUIDs.find(name)};
		if (it == m_nameToUUIDs.end())
			return 0;

		return it->second;
	}



	se::Uint64 UUIDManager::getType(se::UUID uuid) SE_THREAD_SAFE
	{
		auto it {m_UUIDToTypes.find(uuid)};
		if (it == m_UUIDToTypes.end())
			throw std::invalid_argument("Can't find type of uuid " + se::int64ToString(uuid, SE_UUID_BASE));

		return it->second;
	}



	const std::string &UUIDManager::getName(se::UUID uuid) SE_THREAD_SAFE
	{
		for (const auto &name : m_nameToUUIDs)
		{
			if (name.second == uuid)
				return name.first;
		}

		throw std::invalid_argument("Can't find name of uuid " + se::int64ToString(uuid, SE_UUID_BASE));
	}



	bool UUIDManager::isValid(se::UUID uuid) SE_THREAD_SAFE
	{
		auto it {m_UUIDToTypes.find(uuid)};
		return it != m_UUIDToTypes.end();
	}



} // namespace se
