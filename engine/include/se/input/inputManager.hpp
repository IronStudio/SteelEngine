#pragma once



namespace se
{
	class InputManager
	{
		public:
			InputManager() = default;
			virtual ~InputManager() = default;

			virtual void update() = 0;
			virtual bool shouldClose() = 0;
	};



} // namespace se
