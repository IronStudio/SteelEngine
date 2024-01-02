#include "eventManager.hpp"



namespace se
{
	template <typename T, typename ...Args>
	requires std::is_base_of_v<se::Listener, T>
	se::UUID EventManager::addListener(const se::ListenerInfos &infos, Args ...args)
	{
		const se::EventType *type {nullptr};
		if (std::holds_alternative<se::UUID> (infos.type))
		{
			se::UUID typeUUID {std::get<se::UUID> (infos.type)};
			for (auto it {s_types.cbegin()}; it != s_types.cend(); ++it)
			{
				if (it->uuid != typeUUID)
					continue;

				type = &*it;
				break;
			}
		}

		else
			type = std::get<se::EventType*> (infos.type);


		s_listeners[type].push_back(std::make_shared<T> (infos, args...));
		if ((((int)(*s_listeners[type].rbegin())->onMount()) & (int)se::Status::eSuccess) == 0)
			throw std::runtime_error("Can't mount new listener");
		return (*s_listeners[type].rbegin())->getInfos().uuid;
	}



} // namespace se
