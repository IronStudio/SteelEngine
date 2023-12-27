#pragma once

#include <list>
#include <memory>
#include <type_traits>

#include "../core.hpp"
#include "window.hpp"



namespace se
{
	class SE_CORE WindowManager
	{
		public:
			template <class T>
			requires std::is_base_of_v<se::Window, T>
			static se::Window *create(const se::WindowInfos &infos);
			static void remove(const se::Window *window);
			static se::Window *getFromUUID(se::UUID uuid);

			static void update();
			static void unload();

			static inline std::list<se::Window*> getWindows();


		private:
			static std::list<std::unique_ptr<se::Window>> s_windows;
	};



} // namespace se



#include "windowManager.inl"