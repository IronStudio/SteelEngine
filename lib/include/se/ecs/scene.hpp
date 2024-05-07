#pragma once

#include <unordered_map>
#include <vector>

#include "se/core.hpp"
#include "se/memory/poolAllocator.hpp"
#include "se/types.hpp"
#include "se/uuid.hpp"




namespace se::ecs {
	struct ComponentGraphNode {
		se::Uint64 type;
		se::memory::PoolHandle<se::ecs::ComponentGraphNode> hasComponent {};
		se::memory::PoolHandle<se::ecs::ComponentGraphNode> hasNotComponent {};
		std::vector<se::UUID> entities {};
	};


	class SE_CORE Scene {
		public:
			Scene();
			~Scene();

			Scene(const se::ecs::Scene &) = delete;
			const se::ecs::Scene &operator=(const se::ecs::Scene &) = delete;

			Scene(se::ecs::Scene &&scene) noexcept;
			const se::ecs::Scene &operator=(se::ecs::Scene &&scene) noexcept;

			se::UUID addEntity();
			std::vector<se::UUID> addEntities(se::Count count);

			void removeEntity(se::UUID entity);
			void removeEntities(const std::vector<se::UUID> &entities);

			template <typename T>
			T &addComponent(se::UUID entity);
			template <typename T>
			T &getComponent(se::UUID entity);
			template <typename T>
			bool hasComponent(se::UUID entity);
			template <typename T>
			void removeComponent(se::UUID entity);


		private:
			using ComponentCollection = std::vector<se::Byte>;
			std::unordered_map<se::Uint64, ComponentCollection> m_components;
			se::memory::PoolHandle<se::ecs::ComponentGraphNode> m_componentGraph;
			se::memory::PoolAllocator<se::ecs::ComponentGraphNode> m_componentGraphAllocator;
	};

} // namespace se::ecs
