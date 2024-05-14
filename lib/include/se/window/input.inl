#include "se/window/input.hpp"



namespace se::window {
	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	void InputManagerBase<Implementation>::update() {
		s_implementation.update();
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	Implementation InputManagerBase<Implementation>::s_implementation {};


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::isKeyDown(se::window::Key key) {
		return s_implementation.isKeyDown(key);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::isKeyUp(se::window::Key key) {
		return s_implementation.isKeyUp(key);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::wasKeyPressed(se::window::Key key) {
		return s_implementation.wasKeyPressed(key);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::wasKeyReleased(se::window::Key key) {
		return s_implementation.wasKeyReleased(key);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::isMouseButtonDown(se::window::MouseButton mouseButton) {
		return s_implementation.isMouseButtonDown(mouseButton);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::isMouseButtonUp(se::window::MouseButton mouseButton) {
		return s_implementation.isMouseButtonUp(mouseButton);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::wasMouseButtonPressed(se::window::MouseButton mouseButton) {
		return s_implementation.wasMouseButtonPressed(mouseButton);
	}


	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	bool InputManagerBase<Implementation>::wasMouseButtonReleased(se::window::MouseButton mouseButton) {
		return s_implementation.wasMouseButtonReleased(mouseButton);
	}


} // namespace se::window
