#pragma once

#include <map>
#include <mutex>
#include <string>

#include "core.hpp"
#include "status.hpp"
#include "types.hpp"



namespace se
{
	using UUID = se::Uint32;



	class SE_CORE UUIDManager
	{
		public:
			template <typename T>
			static se::UUID generate(T *object);
			template <typename T>
			static se::UUID generate(const std::string &name, T *object);
			static void remove(se::UUID uuid);

			static bool isValid(se::UUID uuid);
			static bool isValid(const std::string &name);
			template <typename T>
			static se::UUID getUUID(const T *object);
			static se::UUID getUUID(const std::string &name);

			static se::Type getType(UUID uuid);
			template <typename T>
			static T *getObject(UUID uuid);


		private:
			struct Data
			{
				void *object;
				se::Type type;
			};

			template <typename T>
			static se::Status s_insertUUID(se::UUID uuid, T *object);
			static se::UUID s_convertStringToUUID(const std::string &text);

			static std::map<se::UUID, se::UUIDManager::Data> s_uuids;
			static se::UUID s_lastGeneratedUUID;
	};

} // namespace se



#include "uuid.inl"