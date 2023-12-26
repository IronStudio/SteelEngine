#include "registry.hpp"



namespace se
{
	template <typename ...Args>
	requires (sizeof...(Args) >= 1)
	std::vector<se::Entity*> Registry::query()
	{
		if constexpr (sizeof...(Args) == 1)
		{
			using T = std::tuple_element<0, std::tuple<Args...>>::type;

			std::vector<se::Entity*> entities {};
			auto &list {m_sortedByComponents[typeid(T).hash_code()]};
			entities.reserve(list.size());

			for (auto it {list.begin()}; it != list.end(); ++it)
				entities.push_back(*it);

			return entities;
		}

		else
		{
			std::vector<se::Entity*> entities {};
			std::array<std::vector<se::Entity*>, sizeof...(Args)> elements {};
			se::Length i {0};

			([&] () {
				elements[i] = this->query<Args> ();
				++i;
			} (), ...);

			for (auto it {elements[0].begin()}; it != elements[0].end(); ++it)
			{
				bool founded {true};

				for (i = 1; i < elements.size(); ++i)
				{
					auto find {std::find(elements[i].begin(), elements[i].end(), *it)};
					if (find == elements[i].end())
					{
						founded = false;
						break;
					}
				}

				if (founded)
					entities.push_back(*it);
			}

			return entities;
		}
	}



} // namespace se
