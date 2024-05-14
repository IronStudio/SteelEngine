#pragma once

#include "se/core.hpp"



namespace se {
	class SE_CORE Engine {
		public:
			static void reset();

			static void shutdown();
			static bool isRunning();

			static void activateReboot();
			static bool isRebootActivated();

		private:
			static bool s_running;
			static bool s_reboot;
	};

} // namespace se
