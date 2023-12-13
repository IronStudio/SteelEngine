#include "component.hpp"



namespace se
{
	Component::Component(se::Int32 type) :
		m_type {type},
		m_children {}
	{

	}



	se::Int32 Component::getType() const noexcept
	{
		return m_type;
	}



	template <class T>
	T &Component::useAs() const noexcept
	{
		return *reinterpret_cast<T*> (this);
	}



	void Component::addChild(se::Component *child)
	{
		m_children.push_back(child);
	}



	void Component::removeChild(se::Component *child)
	{
		m_children.remove(child);
	}



	const std::list<se::Component*> &Component::getChildren() const noexcept
	{
		return m_children;
	}



	bool Component::canBeChild() const noexcept
	{
		return true;
	}



	bool Component::canBeParent() const noexcept
	{
		return true;
	}



} // namespace se
