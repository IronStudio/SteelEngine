#include "layer.hpp"

#include <algorithm>

#include "logging.hpp"



namespace se
{
	std::list<se::LayerInfos> LayerManager::s_layers {};



	se::UUID LayerManager::addLayer(se::LayerInfos infos)
	{
		infos.uuid = se::UUIDManager::generate<se::LayerInfos> ();
		if ((se::Length)infos.level >= s_layers.size() || infos.level < 0)
		{
			s_layers.push_back(infos);
			return infos.uuid;
		}
		
		auto it {s_layers.begin()};
		std::advance(it, infos.level);
		s_layers.insert(it, infos);

		return infos.uuid;
	}



	const se::LayerInfos &LayerManager::getLayer(se::UUID uuid)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid == uuid)
				return *it;
		}

		throw std::invalid_argument("Can't find wanted layer");
	}



	void LayerManager::toggleLayer(se::UUID uuid)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid == uuid)
			{
				it->enabled = !it->enabled;
				return;
			}
		}

		throw std::invalid_argument("Can't toggle wanted layer");
	}



	void LayerManager::forEachEnabledLayer(const std::function<bool(const se::LayerInfos &)> &callback)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (!it->enabled)
				continue;

			if (callback(*it))
				return;
		}
	}



	void LayerManager::moveLayerToLevel(se::UUID uuid, se::Int32 level)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid != uuid)
				continue;

			if (it->level == level)
				return;

			if ((se::Length)level >= s_layers.size() || level < 0)
			{
				it->level = s_layers.size() - 1;
				s_layers.insert(s_layers.end(), *it);
				s_layers.erase(it);
				break;
			}
			
			auto position {s_layers.begin()};
			std::advance(position, level);
			s_layers.insert(position, *it);
			s_layers.erase(it);
			break;
		}

		LayerManager::s_recalculateLevel();
	}



	void LayerManager::moveLayerUp(se::UUID uuid)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid != uuid)
				continue;

			if (it->level == 0)
				return;
			
			auto position {s_layers.begin()};
			std::advance(position, it->level - 1);
			s_layers.insert(position, *it);
			s_layers.erase(it);
			break;
		}

		LayerManager::s_recalculateLevel();
	}



	void LayerManager::moveLayerDown(se::UUID uuid)
	{
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			if (it->uuid != uuid)
				continue;

			if ((se::Length)it->level == s_layers.size() - 1)
				return;
			
			auto position {s_layers.begin()};
			std::advance(position, it->level + 1);
			s_layers.insert(position, *it);
			s_layers.erase(it);
			break;
		}

		LayerManager::s_recalculateLevel();
	}



	void LayerManager::s_recalculateLevel()
	{
		se::Int32 level {0};
		for (auto it {s_layers.begin()}; it != s_layers.end(); ++it)
		{
			it->level = level++;
			SE_CORE_DEBUG("Layer %s : level %d (%d)", it->name.c_str(), it->level, level - 1);
		}
	}



} // namespace se
