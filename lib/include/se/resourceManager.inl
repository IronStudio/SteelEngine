#include "se/resourceManager.hpp"



namespace se {

	template <typename T>
	Resource<T>::~Resource() {
		if (uuid != 0 && res != nullptr)
			se::ResourceManager::unload(*this);
	}




	template <typename T>
	static void unload(const se::Resource<T> &res) {
		delete res.res;
	}


} // namespace se
