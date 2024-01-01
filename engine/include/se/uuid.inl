#include "uuid.hpp"

#include <stdexcept>

#include "utils/hash.hpp"



namespace se
{
	template <typename T>
	se::UUID UUIDManager::generate(T *object) SE_THREAD_SAFE
	{
		std::lock_guard _ {s_mutex};
		++s_lastGeneratedUUID;
		if (s_lastGeneratedUUID >= std::numeric_limits<signed se::UUID>::max())
		{
			s_lastGeneratedUUID = 1;
			for (const auto &uuid : s_uuids)
			{
				if (uuid.first > s_lastGeneratedUUID)
					break;

				++s_lastGeneratedUUID;
			}
		}

		auto result {UUIDManager::s_insertUUID<T> (s_lastGeneratedUUID, object)};
		SE_ASSERT(!(result & se::Status::eDuplicate), "Failed to generate new UUID because of duplicate");
		SE_ASSERT(result & se::Status::eSuccess, "Failed to generate new UUID");

		return s_lastGeneratedUUID;
	}



	template <typename T>
	se::UUID UUIDManager::generate(const std::string &name, T *object) SE_THREAD_SAFE
	{
		se::UUID uuid {UUIDManager::s_convertStringToUUID(name)};
		std::lock_guard _ {s_mutex};
		auto result {UUIDManager::s_insertUUID<T> (uuid, object)};
		SE_ASSERT(!(result & se::Status::eDuplicate), "Failed to generate new named UUID because of duplicate");
		SE_ASSERT(result & se::Status::eSuccess, "Failed to generate new named UUID");

		return uuid;
	}



	template <typename T>
	se::UUID UUIDManager::getUUID(const T *object) SE_THREAD_SAFE
	{
		for (auto it {s_uuids.begin()}; it != s_uuids.end(); ++it)
		{
			if (it->second.type != typeid(T))
				continue;

			if (static_cast<T*> (it->second.object) != object)
				continue;

			return it->first;
		}

		throw std::invalid_argument("Can't find uuid of given object");
	}



	template <typename T>
	T *UUIDManager::getObject(UUID uuid) SE_THREAD_SAFE
	{
		for (auto it {s_uuids.cbegin()}; it != s_uuids.cend(); ++it)
		{
			if (it->first != uuid)
				continue;

			return static_cast<T*> (it->second.object);
		}

		throw std::invalid_argument("Can't find object of the given uuid");
	}



	template <typename T>
	se::Status UUIDManager::s_insertUUID(se::UUID uuid, T *object)
	{
		bool inserted {false};

		for (auto it {s_uuids.cbegin()}; it != s_uuids.cend(); ++it)
		{
			if (it->first < uuid)
				continue;

			if (it->first == uuid)
				return se::Status::eDuplicate;

			s_uuids.insert(it, {uuid, se::UUIDManager::Data(object, typeid(T))});
			inserted = true;
			break;
		}

		if (inserted)
			s_uuids.push_back({uuid, se::UUIDManager::Data(object, typeid(T))});

		return se::Status::eSuccess;
	}



} // namespace se
