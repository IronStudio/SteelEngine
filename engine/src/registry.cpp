#include "registry.hpp"

#include "logging.hpp"



namespace se
{
	Registry::Registry() :
		m_entities {},
		m_sortedByComponents {}
	{

	}



	Registry::~Registry()
	{

	}



	se::Entity *Registry::createEntity()
	{
		m_entities.push_back(se::Entity());
		return &*m_entities.rbegin();
	}



	std::vector<se::Entity*> Registry::createEntities(se::Length count)
	{
		std::vector<se::Entity*> entities {};
		entities.reserve(count);
		for (se::Length i {0}; i < count; ++i)
		{
			m_entities.push_back(se::Entity());
			entities.push_back(&*m_entities.rbegin());
		}

		return entities;
	}



	void Registry::removeEntity(se::UUID uuid)
	{
		se::Entity *entity {nullptr};

		for (auto it {m_entities.begin()}; it != m_entities.end(); ++it)
		{
			if (it->getUUID() != uuid)
				continue;

			entity = &(*it);
			m_entities.erase(it);
			break;
		}

		if (entity == nullptr)
			throw std::invalid_argument("Can't remove an unknown entity from registry");

		for (auto list {m_sortedByComponents.begin()}; list != m_sortedByComponents.end(); ++list)
		{
			for (auto it {list->second.begin()}; it != list->second.end(); ++it)
			{
				if (*it != entity)
					continue;

				list->second.erase(it);
				break;
			}
		}
	}



	void Registry::removeEntities(const std::vector<se::UUID> &uuids)
	{
		for (se::UUID uuid : uuids)
			this->removeEntity(uuid);
	}



	se::Entity *Registry::getEntityFromUUID(se::UUID uuid)
	{
		for (auto it {m_entities.begin()}; it != m_entities.end(); ++it)
		{
			if (it->getUUID() != uuid)
				continue;

			return &*it;
		}

		throw std::invalid_argument("Can't find an unknown entity from registry");
	}



	void Registry::sortEntities()
	{
		for (auto entity {m_entities.begin()}; entity != m_entities.end(); ++entity)
		{
			for (auto component {entity->getComponents().begin()}; component != entity->getComponents().end(); ++component)
			{
				bool founded {false};

				for (const auto &element : m_sortedByComponents[component->type])
				{
					if (&(*entity) != element)
						continue;

					founded = true;
					break;
				}

				if (!founded)
					m_sortedByComponents[component->type].push_back(&(*entity));
			}
		}
	}



} // namespace se
