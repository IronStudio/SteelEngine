#pragma once

#include <map>
#include <mutex>
#include <string>

#include "core.hpp"
#include "types.hpp"



namespace se
{
	using UUID = se::Uint32;



	class SE_CORE UUIDManager
	{
		public:
			template <typename T>
			static se::UUID generate () SE_THREAD_SAFE;
			template <typename T>
			static se::UUID generate (const std::string &name) SE_THREAD_SAFE;

			static se::UUID getUUID(const std::string &name) SE_THREAD_SAFE;
			static const std::type_info &getType(se::UUID uuid) SE_THREAD_SAFE;
			static const std::string &getName(se::UUID uuid) SE_THREAD_SAFE;


		private:
			static std::mutex m_mutex;
			static std::map<std::string, se::UUID> m_nameToUUIDs;
			static std::map<se::UUID, std::type_info> m_UUIDToTypes;
			static se::UUID m_lastGeneratedUnnamedUUID;
	};

} // namespace se



#include "uuid.inl"