#include "layer.hpp"



namespace se
{
	const se::LayerManager::ConstIterator &LayerManager::cbegin()
	{
		return s_enabledLayers.cbegin();
	}



	const se::LayerManager::ConstIterator &LayerManager::cend()
	{
		return s_enabledLayers.cend();
	}



	const se::LayerManager::ConstReverseIterator &LayerManager::crbegin()
	{
		return s_enabledLayers.crbegin();
	}



	const se::LayerManager::ConstReverseIterator &LayerManager::crend()
	{
		return s_enabledLayers.crend();
	}



	std::mutex &LayerManager::getIterationMutex()
	{
		return s_mutex;
	}



} // namespace se
