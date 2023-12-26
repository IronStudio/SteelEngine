#pragma once

#include <functional>
#include <list>
#include <string>

#include "core.hpp"
#include "uuid.hpp"



namespace se
{
	struct LayerInfos
	{
		std::string name;
		se::Int32 level;
		se::UUID uuid;
		bool enabled;
	};


	
	/**
	 * @brief A global static class that handles layers
	*/
	class SE_CORE LayerManager
	{
		public:
			static se::UUID addLayer(se::LayerInfos infos);

			static const se::LayerInfos &getLayer(se::UUID uuid);
			static void toggleLayer(se::UUID uuid);

			static void forEachEnabledLayer(const std::function<bool(const se::LayerInfos &)> &callback);
			static void moveLayerToLevel(se::UUID uuid, se::Int32 level);
			static void moveLayerUp(se::UUID uuid);
			static void moveLayerDown(se::UUID uuid);

		
		private:
			static void s_recalculateLevel();

			static std::list<se::LayerInfos> s_layers;
	};


} // namespace se
