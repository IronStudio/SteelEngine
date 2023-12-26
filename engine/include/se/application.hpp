#pragma once

#include <string>

#include "core.hpp"



namespace se
{
	class SE_CORE Application
	{
		public:
			Application() = default;
			virtual ~Application() = default;

			virtual void load();
			virtual void unload();
			virtual void error(const std::string &msg);
			virtual void run();
	};

} // namespace se



#include "entryPoint.inl"