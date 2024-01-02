#include "layer.hpp"



namespace se
{
	se::LayerManager::ConstIterator LayerManager::cbegin()
	{
		return s_enabledLayers.cbegin();
	}



	se::LayerManager::ConstIterator LayerManager::cend()
	{
		return s_enabledLayers.cend();
	}



	se::LayerManager::ConstReverseIterator LayerManager::crbegin()
	{
		return s_enabledLayers.crbegin();
	}



	se::LayerManager::ConstReverseIterator LayerManager::crend()
	{
		return s_enabledLayers.crend();
	}



} // namespace se
