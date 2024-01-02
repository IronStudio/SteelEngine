#include "layer.hpp"

#include "utils/assert.hpp"



namespace se
{
	std::list<const se::LayerInfos*> LayerManager::s_enabledLayers {};
	std::list<se::LayerInfos> LayerManager::s_layers {};



	se::UUID LayerManager::add(se::LayerInfos layerInfos)
	{
		if (layerInfos.level > s_layers.size() || layerInfos.level == se::layerLastPlace)
			layerInfos.level = s_layers.size();

		SE_ASSERT(layerInfos.level >= 0, "Can't have negative layer level");

		auto insertionPlace {s_layers.cend()};
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->level <= layerInfos.level)
				continue;

			insertionPlace = it;
			break;
		}

		auto current {s_layers.insert(insertionPlace, layerInfos)};
		current->uuid = se::UUIDManager::generate<se::LayerInfos> (&*current);
		
		if (current->enabled)
		{
			auto refInsertionPlace {s_enabledLayers.cend()};
			
			for (auto it {s_enabledLayers.cbegin()}; it != s_enabledLayers.cend(); ++it)
			{
				if ((*it)->level <= layerInfos.level)
					continue;

				refInsertionPlace = it;
				break;
			}

			s_enabledLayers.insert(refInsertionPlace, &*current);
		}

		return current->uuid;
	}



	void LayerManager::remove(se::UUID layer)
	{
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->uuid != layer)
				continue;

			if (it->enabled)
			{
				for (auto enabled {s_enabledLayers.cbegin()}; enabled != s_enabledLayers.cend(); ++enabled)
				{
					if ((*enabled)->uuid != layer)
						continue;

					s_enabledLayers.erase(enabled);
					break;
				}
			}

			s_layers.erase(it);
			break;
		}
	}



	void LayerManager::toggle(se::UUID layer)
	{
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->uuid != layer)
				continue;

			if (it->enabled)
			{
				se::LayerManager::s_disable(it);
				return;
			}

			se::LayerManager::s_enable(it);
			return;
		}

		throw std::invalid_argument("Can't toggle layer that does not exist");
	}



	void LayerManager::enable(se::UUID layer)
	{
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->uuid != layer)
				continue;

			se::LayerManager::s_enable(it);
			return;
		}

		throw std::invalid_argument("Can't enable layer that does not exist");
	}



	void LayerManager::disable(se::UUID layer)
	{
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->uuid != layer)
				continue;

			se::LayerManager::s_disable(it);
			return;
		}

		throw std::invalid_argument("Can't disable layer that does not exist");
	}



	void LayerManager::moveUp(se::UUID layer)
	{
		se::LayerManager::s_swap(layer, true);
	}



	void LayerManager::moveDown(se::UUID layer)
	{
		se::LayerManager::s_swap(layer, false);
	}



	const se::LayerInfos &LayerManager::get(se::UUID layer)
	{
		for (auto it {s_layers.cbegin()}; it != s_layers.cend(); ++it)
		{
			if (it->uuid != layer)
				continue;

			return *it;
		}

		throw std::invalid_argument("Can't get the given layer because it was not found");
	}



	void LayerManager::s_enable(const std::list<se::LayerInfos>::const_iterator &layer)
	{
		if (layer->enabled)
			return;

		auto insertionPlace {s_enabledLayers.cend()};

		for (auto it {s_enabledLayers.cbegin()}; it != s_enabledLayers.cend(); ++it)
		{
			if ((*it)->level <= layer->level)
				continue;

			insertionPlace = it;
			break;
		}

		s_enabledLayers.insert(insertionPlace, &*layer);
	}



	void LayerManager::s_disable(const std::list<se::LayerInfos>::const_iterator &layer)
	{
		if (!layer->enabled)
			return;

		for (auto it {s_enabledLayers.cbegin()}; it != s_enabledLayers.cend(); ++it)
		{
			if ((*it)->uuid != layer->uuid)
				continue;

			s_enabledLayers.erase(it);
			return;
		}

		throw std::invalid_argument("ERROR : A layer was set to be enabled but is not in the enabled list. Unable to disable him");
	}



	void LayerManager::s_swap(se::UUID layer, bool withPrevious)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid != layer)
				continue;

			auto motionFunction {std::next<std::list<se::LayerInfos>::iterator>};
			if (withPrevious)
				motionFunction = std::prev<std::list<se::LayerInfos>::iterator>;

			auto toSwap = motionFunction(it, 1);
			se::Int8 oldLevel {it->level};
			it->level = toSwap->level;
			toSwap->level = oldLevel;
			std::swap(it, toSwap);

			if (!it->enabled || !toSwap->enabled)
				return;

			auto first {s_enabledLayers.cbegin()};
			auto second {s_enabledLayers.cbegin()};

			se::Uint8 founded {0};

			for (auto enabled {s_enabledLayers.cbegin()}; enabled != s_enabledLayers.cend(); ++enabled)
			{
				if ((*enabled)->uuid == it->uuid)
				{
					first = enabled;
					++founded;
				}

				if ((*enabled)->uuid == toSwap->uuid)
				{
					second = enabled;
					++founded;
				}

				if (founded == 2)
					break;
			}

			std::swap(first, second);
			return;
		}
	}



} // namespace se
