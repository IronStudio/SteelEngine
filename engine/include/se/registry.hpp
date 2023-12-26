#pragma once

#include <list>
#include <map>

#include "entity.hpp"



namespace se
{
	class SE_CORE Registry final
	{
		public:
			Registry();
			~Registry();

			se::Entity *createEntity();
			std::vector<se::Entity*> createEntities(se::Length count);
			void removeEntity(se::UUID uuid);
			void removeEntities(const std::vector<se::UUID> &uuids);
			se::Entity *getEntityFromUUID(se::UUID uuid);

			void sortEntities();

			template <typename ...Args>
			requires (sizeof...(Args) >= 1)
			std::vector<se::Entity*> query();


		private:
			std::list<se::Entity> m_entities;
			std::map<se::Uint64, std::list<se::Entity*>> m_sortedByComponents;
	};

} // namespace se



#include "registry.inl"