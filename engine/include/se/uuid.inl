#include "uuid.hpp"

#include <stdexcept>

#include "utils/hash.hpp"



namespace se
{
	template <typename T>
	se::UUID UUIDManager::generate() SE_THREAD_SAFE
	{
		std::lock_guard<std::mutex> _ {m_mutex};
		if (m_lastGeneratedUnnamedUUID >= std::numeric_limits<se::Int32>::max() - 1)
		{
			for (se::UUID i {1}; i < std::numeric_limits<se::Int32>::max(); ++i)
			{
				if (m_UUIDToTypes.find(i) == m_UUIDToTypes.end())
				{
					m_UUIDToTypes[i] = typeid(T).hash_code();
					m_lastGeneratedUnnamedUUID = i;
					return i;
				}
			}

			throw std::overflow_error("There is no more space to generate unnamed UUID");
		}

		++m_lastGeneratedUnnamedUUID;
		m_UUIDToTypes[m_lastGeneratedUnnamedUUID] = typeid(T).hash_code();
		return m_lastGeneratedUnnamedUUID;
	}



	template <typename T>
	se::UUID UUIDManager::generate(const std::string &name) SE_THREAD_SAFE
	{
		se::UUID uuid {se::hash(name) | (1 << 31)};
		std::lock_guard<std::mutex> _ {m_mutex};

		if (m_nameToUUIDs.find(name) != m_nameToUUIDs.end())
			throw std::overflow_error("Please change UUID name '" + name + "' to something different because there is overlap in the hash function");

		m_UUIDToTypes[uuid] = typeid(T).hash_code();
		m_nameToUUIDs[name] = uuid;
		return uuid;
	}



} // namespace se
