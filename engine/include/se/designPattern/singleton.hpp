#pragma once



namespace se::designPattern
{
	/**
	 * @brief A class whose purpose is to create singleton of other classes
	 * @tparam T The class that you want to use as a singleton
	 * 
	 * To use this class, you should make T inherited **publicly** from Singleton<T>, make `Singleton<T>` a friend of `T`,
	 * create only `T` default constructor and destructor in protected scope. Please consider deleting copy and move constructor
	 * / `operator=` to maximise safety. You can see an example of singleton in `se/logger.hpp`
	*/
	template <class T>
	class Singleton
	{
		public:
			/**
			 * @brief Load the single instance of the class
			 * @warning Call it only once
			*/
			static void load();
			/**
			 * @brief Unload a loaded single instance
			 * @warning Don't forget to call it to release memory
			*/
			static void unload();
			/**
			 * @brief Get a reference to the single instance
			 * @warning Make sure to have call `Singleton<T>::load()` before calling `Singleton<T>::get()`
			*/
			static T &get();
			

		protected:
			Singleton() = default;
			virtual ~Singleton() = default;


		private:
			static T *s_instance;

			Singleton(const se::designPattern::Singleton<T> &) = delete;
			const se::designPattern::Singleton<T> &operator=(const se::designPattern::Singleton<T> &) = delete;
			Singleton(se::designPattern::Singleton<T> &&) noexcept = delete;
			const se::designPattern::Singleton<T> &operator=(se::designPattern::Singleton<T> &&) noexcept = delete;
	};

} // namespace se::designPattern



#include "singleton.inl"