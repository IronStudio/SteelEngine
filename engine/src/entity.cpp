#include "entity.hpp"



namespace se
{
	Entity::Entity() :
		m_uuid {se::UUIDManager::generate<se::Entity> ()},
		m_components {}
	{

	}



	Entity::~Entity()
	{
		
	}



} // namespace se
