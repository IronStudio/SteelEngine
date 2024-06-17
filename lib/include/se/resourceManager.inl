#include "se/resourceManager.hpp"



namespace se {

	template <typename T>
	Resource<T>::Resource() :
		uuid {0},
		res {nullptr}
	{

	}


	template <typename T>
	Resource<T>::Resource(se::Resource<T> &&res) noexcept :
		uuid {res.uuid},
		res {res.res}
	{
		res.uuid = 0;
		res.res = nullptr;
	}


	template <typename T>
	const se::Resource<T> &Resource<T>::operator=(se::Resource<T> &&res) noexcept {
		uuid = res.uuid;
		this->res = res.res;
		res.uuid = 0;
		res.res = nullptr;
		return *this;
	}


	template <typename T>
	Resource<T>::~Resource() {
		if (uuid != 0 && res != nullptr)
			se::ResourceManager::unload(*this);
	}




	template <typename T>
	void ResourceManager::unload(se::Resource<T> &res) {
		delete res.res;
		res.res = nullptr;
		res.uuid = 0;
	}


} // namespace se
