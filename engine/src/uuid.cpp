#include "uuid.hpp"

#include "utils/assert.hpp"
#include "utils/convert.hpp"



namespace se
{
	std::mutex UUIDManager::s_mutex {};
	std::map<se::UUID, se::UUIDManager::Data> UUIDManager::s_uuids {};
	se::UUID UUIDManager::s_lastGeneratedUUID {0};



	void UUIDManager::remove(se::UUID uuid) SE_THREAD_SAFE
	{
		for (auto it {s_uuids.cbegin()}; it != s_uuids.cend(); ++it)
		{
			if (it->first != uuid)
				continue;

			s_uuids.erase(it);
			return;
		}


	}



	bool UUIDManager::isValid(se::UUID uuid) SE_THREAD_SAFE
	{
		return s_uuids.find(uuid) != s_uuids.end();
	}



	bool UUIDManager::isValid(const std::string &name) SE_THREAD_SAFE
	{
		return s_uuids.find(s_convertStringToUUID(name)) != s_uuids.end();
	}



	se::UUID UUIDManager::getUUID(const std::string &name) SE_THREAD_SAFE
	{
		const se::UUID uuid {UUIDManager::s_convertStringToUUID(name)};
		SE_ASSERT(UUIDManager::isValid(uuid), "Can't find UUID of given name");
		return uuid;
	}



	se::Type UUIDManager::getType(UUID uuid) SE_THREAD_SAFE
	{
		for (auto it {s_uuids.cbegin()}; it != s_uuids.cend(); ++it)
		{
			if (it->first != uuid)
				continue;

			return it->second.type;
		}

		throw std::invalid_argument("Can't find type of the given UUID");
	}



	se::UUID UUIDManager::s_convertStringToUUID(const std::string &text)
	{
		return se::hash(text) | (1 << 31);
	}



} // namespace se
