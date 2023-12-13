#pragma once

#include <list>

#include "essentials/intTypes.hpp"



namespace se
{
	using ComponentType = se::Int32;
	enum class ComponentTypeFlags
	{
		unknown = -1,
		root = 0,
		script = 1,
	};


	class Component
	{
		public:
			inline Component(se::Int32 type);
			virtual ~Component() = default;

			inline se::Int32 getType() const noexcept;

			template <class T>
			T &useAs() const noexcept;

			inline void addChild(se::Component *child);
			inline void removeChild(se::Component *child);
			inline const std::list<se::Component*> &getChildren() const noexcept;

			virtual inline bool canBeChild() const noexcept;
			virtual inline bool canBeParent() const noexcept;



		protected:
			se::Int32 m_type;
			std::list<se::Component*> m_children;
	};

} // namespace se



#include "component.inl"