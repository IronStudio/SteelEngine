#include "se/engine.hpp"



namespace se {
	void Engine::reset() {
		s_running = true;
		s_reboot = false;
	}


	void Engine::shutdown() {
		s_running = false;
	}


	bool Engine::isRunning() {
		return s_running;
	}


	void Engine::activateReboot() {
		s_reboot = true;
	}


	bool Engine::isRebootActivated() {
		return s_reboot;
	}


	bool Engine::s_running {true};
	bool Engine::s_reboot {false};

} // namespace se
