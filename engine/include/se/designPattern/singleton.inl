#include "../essentials/assertion.hpp"
#include "singleton.hpp"



namespace se::designPattern
{
	template <class T>
	T *Singleton<T>::s_instance {nullptr};



	template <class T>
	void Singleton<T>::load()
	{
		SE_UNKNOWN_ASSERT(s_instance == nullptr, "Can't load already loaded singleton instance");
		s_instance = new T();
	}



	template <class T>
	void Singleton<T>::unload()
	{
		if (s_instance != nullptr)
			delete s_instance;
	}



	template <class T>
	T &Singleton<T>::get()
	{
		SE_UNKNOWN_ASSERT(s_instance != nullptr, "Can't get not loaded singleton instance");
		return *s_instance;
	}



} // namespace se::designPattern
