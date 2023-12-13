#pragma once

#include "../component.hpp"



namespace se::components
{
	class Script : public se::Component
	{
		public:
			inline Script();
			virtual ~Script() override = default;

			virtual void run() = 0;
	};



} // namespace se::components



#include "script.inl"