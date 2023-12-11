#pragma once



namespace se::designPattern
{
	template <class T>
	class Singleton
	{
		public:
			static void load();
			static void unload();
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