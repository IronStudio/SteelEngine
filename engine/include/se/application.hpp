#pragma once

#include <string>

#include "core.hpp"



namespace se
{
	/**
	 * @brief Base class of an application
	*/
	class SE_CORE Application
	{
		public:
			Application() = default;
			virtual ~Application() = default;

			/**
			 * @warning When overloading, don't forget to call `se::Application::load()`
			*/
			virtual void load();
			/**
			 * @warning When overloading, don't forget to call `se::Application::load()`
			*/
			virtual void unload();
			virtual void error(const std::string &msg);
			virtual void run();
	};

} // namespace se



#include "entryPoint.inl"