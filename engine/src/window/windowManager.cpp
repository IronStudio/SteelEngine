#include "window/windowManager.hpp"

#include <algorithm>
#include <stdexcept>



namespace se
{
	std::list<std::unique_ptr<se::Window>> WindowManager::s_windows {};



	void WindowManager::remove(const se::Window *window)
	{
		auto it {s_windows.begin()};
		for (; it != s_windows.end(); ++it)
		{
			if (it->get() == window)
				break;
		}
		
		if (it == s_windows.end())
			throw std::invalid_argument("Can't remove unknown window");

		s_windows.erase(it);
	}



	se::Window *WindowManager::getFromUUID(se::UUID uuid)
	{
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it)
		{
			if (it->get()->getUUID() != uuid)
				continue;

			return it->get();
		}

		throw std::invalid_argument("Can't find window with the given UUID");
	}



	void WindowManager::update()
	{
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it)
			it->get()->updateInfos();
	}



	void WindowManager::unload()
	{
		s_windows.clear();
	}



} // namespace se
