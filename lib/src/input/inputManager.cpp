#include "se/input/inputManager.hpp"

#include <map>

#include <SDL2/SDL.h>

#include "se/assertion.hpp"
#include "se/engine.hpp"
#include "se/logger.hpp"
#include "se/window/windowManager.hpp"



namespace se::input {
	se::input::Key keySDL2ToSe(SDL_Scancode key);
	se::input::MouseButton mouseButtonSDL2ToSe(se::Uint8 mouseButton);


	void InputManager::update() {
		static bool firstTime {true};
		if (firstTime) {
			firstTime = false;
			s_load();
		}

		SDL_Event event {};
		s_oldKeyStates = s_keyStates;
		s_oldMouseButtonStates = s_mouseButtonStates;
		s_mouseMotion = se::Vec2i(0, 0);

		for (auto &window : s_wasWindowResized)
			window.second = false;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					s_keyStates[keySDL2ToSe(event.key.keysym.scancode)] = true;
					break;

				case SDL_KEYUP:
					s_keyStates[keySDL2ToSe(event.key.keysym.scancode)] = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					s_mouseButtonStates[mouseButtonSDL2ToSe(event.button.button)] = true;
					break;

				case SDL_MOUSEBUTTONUP:
					s_mouseButtonStates[mouseButtonSDL2ToSe(event.button.button)] = false;
					break;

				case SDL_MOUSEMOTION:
					s_mousePosition = {event.motion.x, event.motion.y};
					s_mouseMotion = {event.motion.xrel, event.motion.yrel};
					break;

				case SDL_WINDOWEVENT:
					s_handleWindowEvent(event);
					break;

				case SDL_QUIT:
					se::Engine::shutdown();
					break;
			}
		}
	}


	bool InputManager::isKeyDown(se::input::Key key) {
		return s_keyStates[key];
	}


	bool InputManager::isKeyUp(se::input::Key key) {
		return !s_keyStates[key];
	}


	bool InputManager::wasKeyPressed(se::input::Key key) {
		return s_keyStates[key] && !s_oldKeyStates[key];
	}


	bool InputManager::wasKeyReleased(se::input::Key key) {
		return !s_keyStates[key] && s_oldKeyStates[key];
	}


	bool InputManager::isMouseButtonDown(se::input::MouseButton button) {
		return s_mouseButtonStates[button];
	}


	bool InputManager::isMouseButtonUp(se::input::MouseButton button) {
		return !s_mouseButtonStates[button];
	}


	bool InputManager::wasMouseButtonPressed(se::input::MouseButton button) {
		return s_mouseButtonStates[button] && !s_oldMouseButtonStates[button];
	}


	bool InputManager::wasMouseButtonReleased(se::input::MouseButton button) {
		return !s_mouseButtonStates[button] && s_oldMouseButtonStates[button];
	}


	se::UUID InputManager::getFocusedWindowUUID() {
		return s_focusedWindowUUID;
	}


	bool InputManager::wasWindowResized(se::UUID uuid) {
		auto it {s_wasWindowResized.find((size_t)uuid)};
		if (it == s_wasWindowResized.end())
			return false;
		return it->second;
	}


	const se::Vec2i &InputManager::getMousePosition() {
		return s_mousePosition;
	}


	const se::Vec2i &InputManager::getMouseMotion() {
		return s_mouseMotion;
	}


	bool InputManager::hasMouseMoved() {
		return s_mouseMotion != se::Vec2i(0, 0);
	}


	void InputManager::s_load() {
		for (se::Count i {0}; i < (se::Count)se::input::Key::__last; ++i)
			s_keyStates[(se::input::Key)i] = false;
		s_oldKeyStates = s_keyStates;

		for (se::Count i {0}; i < (se::Count)se::input::MouseButton::__last; ++i)
			s_mouseButtonStates[(se::input::MouseButton)i] = false;
		s_oldMouseButtonStates = s_mouseButtonStates;

		SDL_Window *focusedWindow {SDL_GetKeyboardFocus()};
		if (focusedWindow != nullptr)
			s_focusedWindowUUID = se::window::WindowManager::getWindow(focusedWindow).getUUID();
	}


	void InputManager::s_handleWindowEvent(const SDL_Event &event) {
		switch (event.window.event) {
			case SDL_WINDOWEVENT_TAKE_FOCUS:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				s_focusedWindowUUID = se::window::WindowManager::getWindow(SDL_GetWindowFromID(event.window.windowID)).getUUID();
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				if (!se::window::WindowManager::hasWindow(SDL_GetWindowFromID(event.window.windowID)))
					break;
				else if (s_focusedWindowUUID == se::window::WindowManager::getWindow(SDL_GetWindowFromID(event.window.windowID)).getUUID())
					s_focusedWindowUUID = 0;
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				se::window::WindowManager::getWindow(SDL_GetWindowFromID(event.window.windowID)).sync(se::window::WindowResync::eSize);
				s_wasWindowResized[(size_t)se::window::WindowManager::getWindow(SDL_GetWindowFromID(event.window.windowID)).getUUID()] = true;
				break;

			case SDL_WINDOWEVENT_MOVED:
				se::window::WindowManager::getWindow(SDL_GetWindowFromID(event.window.windowID)).sync(se::window::WindowResync::ePosition);
				break;

			case SDL_WINDOWEVENT_CLOSE:
				se::window::WindowManager::destroyWindow(SDL_GetWindowFromID(event.window.windowID));
				break;
		}
	}


	std::unordered_map<se::input::Key, bool> InputManager::s_keyStates {};
	std::unordered_map<se::input::Key, bool> InputManager::s_oldKeyStates {};
	std::unordered_map<se::input::MouseButton, bool> InputManager::s_mouseButtonStates {};
	std::unordered_map<se::input::MouseButton, bool> InputManager::s_oldMouseButtonStates {};
	se::UUID InputManager::s_focusedWindowUUID {0};
	std::map<size_t, bool> InputManager::s_wasWindowResized {};
	se::Vec2i InputManager::s_mousePosition {};
	se::Vec2i InputManager::s_mouseMotion {};

	
	se::input::Key keySDL2ToSe(SDL_Scancode key) {
		static const std::unordered_map<SDL_Scancode, se::input::Key> keyMap {
			{SDL_SCANCODE_A,         se::input::Key::eA},
			{SDL_SCANCODE_B,         se::input::Key::eB},
			{SDL_SCANCODE_C,         se::input::Key::eC},
			{SDL_SCANCODE_D,         se::input::Key::eD},
			{SDL_SCANCODE_E,         se::input::Key::eE},
			{SDL_SCANCODE_F,         se::input::Key::eF},
			{SDL_SCANCODE_G,         se::input::Key::eG},
			{SDL_SCANCODE_H,         se::input::Key::eH},
			{SDL_SCANCODE_I,         se::input::Key::eI},
			{SDL_SCANCODE_J,         se::input::Key::eJ},
			{SDL_SCANCODE_K,         se::input::Key::eK},
			{SDL_SCANCODE_L,         se::input::Key::eL},
			{SDL_SCANCODE_M,         se::input::Key::eM},
			{SDL_SCANCODE_N,         se::input::Key::eN},
			{SDL_SCANCODE_O,         se::input::Key::eO},
			{SDL_SCANCODE_P,         se::input::Key::eP},
			{SDL_SCANCODE_Q,         se::input::Key::eQ},
			{SDL_SCANCODE_R,         se::input::Key::eR},
			{SDL_SCANCODE_S,         se::input::Key::eS},
			{SDL_SCANCODE_T,         se::input::Key::eT},
			{SDL_SCANCODE_U,         se::input::Key::eU},
			{SDL_SCANCODE_V,         se::input::Key::eV},
			{SDL_SCANCODE_W,         se::input::Key::eW},
			{SDL_SCANCODE_X,         se::input::Key::eX},
			{SDL_SCANCODE_Y,         se::input::Key::eY},
			{SDL_SCANCODE_Z,         se::input::Key::eZ},
			{SDL_SCANCODE_1,         se::input::Key::e1},
			{SDL_SCANCODE_2,         se::input::Key::e2},
			{SDL_SCANCODE_3,         se::input::Key::e3},
			{SDL_SCANCODE_4,         se::input::Key::e4},
			{SDL_SCANCODE_5,         se::input::Key::e5},
			{SDL_SCANCODE_6,         se::input::Key::e6},
			{SDL_SCANCODE_7,         se::input::Key::e7},
			{SDL_SCANCODE_8,         se::input::Key::e8},
			{SDL_SCANCODE_9,         se::input::Key::e9},
			{SDL_SCANCODE_0,         se::input::Key::e0},
			{SDL_SCANCODE_F1,        se::input::Key::eF1},
			{SDL_SCANCODE_F2,        se::input::Key::eF2},
			{SDL_SCANCODE_F3,        se::input::Key::eF3},
			{SDL_SCANCODE_F4,        se::input::Key::eF4},
			{SDL_SCANCODE_F5,        se::input::Key::eF5},
			{SDL_SCANCODE_F6,        se::input::Key::eF6},
			{SDL_SCANCODE_F7,        se::input::Key::eF7},
			{SDL_SCANCODE_F8,        se::input::Key::eF8},
			{SDL_SCANCODE_F9,        se::input::Key::eF9},
			{SDL_SCANCODE_F10,       se::input::Key::eF10},
			{SDL_SCANCODE_F11,       se::input::Key::eF11},
			{SDL_SCANCODE_F12,       se::input::Key::eF12},
			{SDL_SCANCODE_ESCAPE,    se::input::Key::eEscape},
			{SDL_SCANCODE_TAB,       se::input::Key::eTab},
			{SDL_SCANCODE_CAPSLOCK,  se::input::Key::eCapslock},
			{SDL_SCANCODE_LSHIFT,    se::input::Key::eLShift},
			{SDL_SCANCODE_LCTRL,     se::input::Key::eLCtrl},
			{SDL_SCANCODE_LALT,      se::input::Key::eLAlt},
			{SDL_SCANCODE_RALT,      se::input::Key::eRAlt},
			{SDL_SCANCODE_LCTRL,     se::input::Key::eLCtrl},
			{SDL_SCANCODE_RSHIFT,    se::input::Key::eRShift},
			{SDL_SCANCODE_SPACE,     se::input::Key::eSpace},
			{SDL_SCANCODE_RETURN,    se::input::Key::eEnter},
			{SDL_SCANCODE_BACKSPACE, se::input::Key::eBackspace},
			{SDL_SCANCODE_DELETE,    se::input::Key::eDelete},
			{SDL_SCANCODE_INSERT,    se::input::Key::eInsert},
			{SDL_SCANCODE_HOME,      se::input::Key::eHome},
			{SDL_SCANCODE_END,       se::input::Key::eEnd},
			{SDL_SCANCODE_PAGEUP,    se::input::Key::ePageUp},
			{SDL_SCANCODE_PAGEDOWN,  se::input::Key::ePageDown},
			{SDL_SCANCODE_UP,        se::input::Key::eUp},
			{SDL_SCANCODE_DOWN,      se::input::Key::eDown},
			{SDL_SCANCODE_LEFT,      se::input::Key::eLeft},
			{SDL_SCANCODE_RIGHT,     se::input::Key::eRight}
		};

		auto it {keyMap.find(key)};
		SE_ASSERT(it != keyMap.end(), "Can't find the given SDL2 key to convert it to SE key");
		return it->second;
	}

	se::input::MouseButton mouseButtonSDL2ToSe(se::Uint8 mouseButton) {
		static const std::map<se::Uint8, se::input::MouseButton> buttonMap {
			{SDL_BUTTON_LEFT,   se::input::MouseButton::eLeft},
			{SDL_BUTTON_MIDDLE, se::input::MouseButton::eMiddle},
			{SDL_BUTTON_RIGHT,  se::input::MouseButton::eRight},
			{SDL_BUTTON_X1,     se::input::MouseButton::eX1},
			{SDL_BUTTON_X2,     se::input::MouseButton::eX2},
		};
		auto it {buttonMap.find(mouseButton)};
		SE_ASSERT(it != buttonMap.end(), "Can't find the given SDL2 mouse button to convert it to SE mouse button");
		return it->second;
	}

} // namespace se::input
