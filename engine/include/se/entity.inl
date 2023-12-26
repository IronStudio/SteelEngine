#include "entity.hpp"

#include <algorithm>



namespace se
{
	template <typename T>
	void Entity::addComponent(const T &component)
	{
		if (this->hasComponent<T> ())
			throw std::invalid_argument("Can't add twice the same component to an entity");

		m_components.push_back(se::Component(
			std::make_shared<T> (component),
			typeid(T).hash_code()
		));
	}



	template <typename T>
	void Entity::removeComponent()
	{
		for (auto it {m_components.begin()}; it != m_components.end(); ++it)
		{
			if (it->type != typeid(T).hash_code())
				continue;

			m_components.erase(it);
			return;
		}

		throw std::invalid_argument("Can't remove a component that is not present on an entity");
	}



	template <typename ...Args>
	requires (sizeof...(Args) >= 1)
	bool Entity::hasComponent() const noexcept
	{
		int founded {0};
		for (auto it {m_components.begin()}; it != m_components.end(); ++it)
		{
			([&] () {
				if (it->type == typeid(Args).hash_code())
					++founded;
			} (), ...);
		}

		return founded == sizeof...(Args);
	}



	template <typename T>
	T &Entity::getComponent() const noexcept
	{
		for (auto it {m_components.begin()}; it != m_components.end(); ++it)
		{
			if (it->type == typeid(T).hash_code())
				return *reinterpret_cast<T*> (it->component);
		}

		throw std::invalid_argument("Can't get component that is not present on entity");
	}



	se::UUID Entity::getUUID() const noexcept
	{
		return m_uuid;
	}



	const std::vector<se::Component> &Entity::getComponents() const noexcept
	{
		return m_components;
	}



} // namespace se
