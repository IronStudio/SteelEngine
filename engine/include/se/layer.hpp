#pragma once

#include <list>
#include <mutex>
#include <string>

#include "core.hpp"
#include "types.hpp"
#include "uuid.hpp"



namespace se
{
	constexpr se::Int8 layerLastPlace {-1};


	struct LayerInfos
	{
		se::UUID uuid;
		se::Int8 level {se::layerLastPlace};
		std::string name;
		bool enabled {true};
	};



	class SE_CORE LayerManager
	{
		public:
			using ConstIterator = std::list<const se::LayerInfos*>::const_iterator;
			using ConstReverseIterator = std::list<const se::LayerInfos*>::const_reverse_iterator;

			static se::UUID add(se::LayerInfos layerInfos);
			static void remove(se::UUID layer);
			static void toggle(se::UUID layer);
			static void enable(se::UUID layer);
			static void disable(se::UUID layer);
			static void moveUp(se::UUID layer);
			static void moveDown(se::UUID layer);
			static const se::LayerInfos &get(se::UUID layer);

			inline static se::LayerManager::ConstIterator cbegin();
			inline static se::LayerManager::ConstIterator cend();
			inline static se::LayerManager::ConstReverseIterator crbegin();
			inline static se::LayerManager::ConstReverseIterator crend();


		private:
			static void s_enable(const std::list<se::LayerInfos>::const_iterator &layer);
			static void s_disable(const std::list<se::LayerInfos>::const_iterator &layer);
			static void s_swap(se::UUID layer, bool withPrevious);

			static std::list<const se::LayerInfos*> s_enabledLayers;
			static std::list<se::LayerInfos> s_layers;
	};



} // namespace se



#include "layer.inl"