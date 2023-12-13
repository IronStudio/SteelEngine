#include "root.hpp"



namespace se::components
{
	Root::Root() :
		se::Component(static_cast<se::Int32> (se::ComponentTypeFlags::root))
	{

	}



	bool Root::canBeChild() const noexcept
	{
		return false;
	}



	bool Root::canBeParent() const noexcept
	{
		return true;
	}



} // namespace se::components
