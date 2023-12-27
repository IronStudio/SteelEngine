#include "windowManager.hpp"



namespace se
{
	template <class T>
	requires std::is_base_of_v<se::Window, T>
	se::Window *WindowManager::create(const se::WindowInfos &infos)
	{
		s_windows.push_back(std::make_unique<T> (infos));
		return s_windows.rbegin()->get();
	}



	std::list<se::Window*> WindowManager::getWindows()
	{
		std::list<se::Window*> list {};
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it)
			list.push_back(it->get());

		return list;
	}



} // namespace se
