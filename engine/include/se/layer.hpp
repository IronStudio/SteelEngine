#pragma once

#include <list>
#include <mutex>
#include <string>

#include "core.hpp"
#include "types.hpp"
#include "uuid.hpp"



namespace se
{
	constexpr se::Uint8 layerLastPlace {-1};


	struct LayerInfos
	{
		se::UUID uuid;
		se::Uint8 level {se::layerLastPlace};
		std::string name;
		bool enabled {true};
	};



	class SE_CORE LayerManager
	{
		public:
			using ConstIterator = std::list<const se::LayerInfos*>::const_iterator;
			using ConstReverseIterator = std::list<const se::LayerInfos*>::const_reverse_iterator;

			static se::UUID add(se::LayerInfos layerInfos) SE_THREAD_SAFE;
			static void remove(se::UUID layer) SE_THREAD_SAFE;
			static void toggle(se::UUID layer) SE_THREAD_SAFE;
			static void enable(se::UUID layer) SE_THREAD_SAFE;
			static void disable(se::UUID layer) SE_THREAD_SAFE;
			static void moveUp(se::UUID layer) SE_THREAD_SAFE;
			static void moveDown(se::UUID layer) SE_THREAD_SAFE;
			static const se::LayerInfos &get(se::UUID layer) SE_THREAD_SAFE;

			inline static const se::LayerManager::ConstIterator &cbegin();
			inline static const se::LayerManager::ConstIterator &cend();
			inline static const se::LayerManager::ConstReverseIterator &crbegin();
			inline static const se::LayerManager::ConstReverseIterator &crend();

			inline static std::mutex &getIterationMutex();


		private:
			static void s_enable(const std::list<se::LayerInfos>::const_iterator &layer);
			static void s_disable(const std::list<se::LayerInfos>::const_iterator &layer);
			static void s_swap(se::UUID layer, bool withPrevious);

			static std::mutex s_mutex;
			static std::list<const se::LayerInfos*> s_enabledLayers;
			static std::list<se::LayerInfos> s_layers;
	};



} // namespace se



#include "layer.inl"