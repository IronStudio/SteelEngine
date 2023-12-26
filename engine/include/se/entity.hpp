#pragma once

#include <memory>
#include <vector>

#include "core.hpp"
#include "uuid.hpp"



namespace se
{
	struct Component
	{
		std::shared_ptr<void> component;
		se::Uint64 type;
	};

	class SE_CORE Entity final
	{
		public:
			Entity();
			~Entity();

			template <typename T>
			void addComponent(const T &component);
			template <typename T>
			void removeComponent();

			template <typename ...Args>
			requires (sizeof...(Args) >= 1)
			bool hasComponent() const noexcept;
			template <typename T>
			T &getComponent() const noexcept;

			inline se::UUID getUUID() const noexcept;
			inline const std::vector<se::Component> &getComponents() const noexcept;


		private:
			se::UUID m_uuid;
			std::vector<se::Component> m_components;
	};



} // namespace se



#include "entity.inl"