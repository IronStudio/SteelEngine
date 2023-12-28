#include "windowManager.hpp"

#include "../eventManager.hpp"
#include "../logging.hpp"



namespace se
{
	template <class T>
	requires std::is_base_of_v<se::Window, T>
	se::Window *WindowManager::create(const se::WindowInfos &infos)
	{
		s_windows.push_back(std::make_unique<T> (infos));
		auto window {s_windows.rbegin()->get()};
		(void)se::EventManager::addListener<se::LambdaListener> (
			s_windowLayer,
			window->getEventTypes().find(se::InputType::eWindowClosed)->second,
			window->getUUID(),
			[window] (se::EventType /* type */, se::Event /* event */) -> bool {
				se::WindowManager::remove(window);
				return true;
			}
		);
		return window;
	}



	std::list<se::Window*> WindowManager::getWindows()
	{
		std::list<se::Window*> list {};
		for (auto it {s_windows.begin()}; it != s_windows.end(); ++it)
			list.push_back(it->get());

		return list;
	}



} // namespace se
