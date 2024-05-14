#include "se/window/input.hpp"

#ifdef SE_WSI_SDL3
	#include <SDL3/SDL.h>
#endif

#include "se/assertion.hpp"
#include "se/engine.hpp"



namespace se::window {
#ifdef SE_WSI_SDL3
	se::window::Key convertKeySDL3ToSe(SDL_Scancode key);
	se::window::MouseButton convertMouseButtonSDL3ToSe(se::Uint8 mouseButton);

	SDL3InputManager::SDL3InputManager() :
		m_areKeysDown {},
		m_lastAreKeysDown {},
		m_areMouseButtonsDown {},
		m_lastAreMouseButtonsDown {}
	{

	}


	void SDL3InputManager::update() {
		m_lastAreKeysDown = m_areKeysDown;
		m_lastAreMouseButtonsDown = m_areMouseButtonsDown;

		SDL_Event event {};
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_KEY_DOWN:
					m_areKeysDown[convertKeySDL3ToSe(event.key.keysym.scancode)] = true;
					break;
				case SDL_EVENT_KEY_UP:
					m_areKeysDown[convertKeySDL3ToSe(event.key.keysym.scancode)] = false;
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					m_areMouseButtonsDown[convertMouseButtonSDL3ToSe(event.button.button)] = true;
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					m_areMouseButtonsDown[convertMouseButtonSDL3ToSe(event.button.button)] = true;
					break;
				case SDL_EVENT_QUIT:
					se::Engine::shutdown();
					break;
			}
		}
	}


	bool SDL3InputManager::isKeyDown(se::window::Key key) const noexcept {
		auto it {m_areKeysDown.find(key)};
		if (it == m_areKeysDown.end()) return false;
		return it->second;
	}

	bool SDL3InputManager::isKeyUp(se::window::Key key) const noexcept {
		auto it {m_areKeysDown.find(key)};
		if (it == m_areKeysDown.end()) return false;
		return !it->second;
	}

	bool SDL3InputManager::wasKeyPressed(se::window::Key key) const noexcept {
		auto it {m_areKeysDown.find(key)};
		auto it2 {m_lastAreKeysDown.find(key)};
		if (it == m_areKeysDown.end()) return false;
		if (it2 == m_lastAreKeysDown.end()) return true;
		return it->second && !it2->second;
	}

	bool SDL3InputManager::wasKeyReleased(se::window::Key key) const noexcept {
		auto it {m_areKeysDown.find(key)};
		auto it2 {m_lastAreKeysDown.find(key)};
		if (it == m_areKeysDown.end()) return false;
		if (it2 == m_lastAreKeysDown.end()) return true;
		return !it->second && it2->second;
	}

	bool SDL3InputManager::isMouseButtonDown(se::window::MouseButton mouseButton) const noexcept {
		auto it {m_areMouseButtonsDown.find(mouseButton)};
		if (it == m_areMouseButtonsDown.end()) return false;
		return it->second;
	}

	bool SDL3InputManager::isMouseButtonUp(se::window::MouseButton mouseButton) const noexcept {
		auto it {m_areMouseButtonsDown.find(mouseButton)};
		if (it == m_areMouseButtonsDown.end()) return false;
		return !it->second;
	}

	bool SDL3InputManager::wasMouseButtonPressed(se::window::MouseButton mouseButton) const noexcept {
		auto it {m_areMouseButtonsDown.find(mouseButton)};
		auto it2 {m_lastAreMouseButtonsDown.find(mouseButton)};
		if (it == m_areMouseButtonsDown.end()) return false;
		if(it2 == m_lastAreMouseButtonsDown.end()) return true;
		return it->second && !it2->second;
	}

	bool SDL3InputManager::wasMouseButtonReleased(se::window::MouseButton mouseButton) const noexcept {
		auto it {m_areMouseButtonsDown.find(mouseButton)};
		auto it2 {m_lastAreMouseButtonsDown.find(mouseButton)};
		if (it == m_areMouseButtonsDown.end()) return false;
		if(it2 == m_lastAreMouseButtonsDown.end()) return true;
		return !it->second && it2->second;
	}


#endif



#ifdef SE_WSI_SDL3
	se::window::Key convertKeySDL3ToSe(SDL_Scancode key) {
		static const std::unordered_map<SDL_Scancode, se::window::Key> keys {
			{SDL_SCANCODE_A, se::window::Key::eA},
			{SDL_SCANCODE_B, se::window::Key::eB},
			{SDL_SCANCODE_C, se::window::Key::eC},
			{SDL_SCANCODE_D, se::window::Key::eD},
			{SDL_SCANCODE_E, se::window::Key::eE},
			{SDL_SCANCODE_F, se::window::Key::eF},
			{SDL_SCANCODE_G, se::window::Key::eG},
			{SDL_SCANCODE_H, se::window::Key::eH},
			{SDL_SCANCODE_I, se::window::Key::eI},
			{SDL_SCANCODE_J, se::window::Key::eJ},
			{SDL_SCANCODE_K, se::window::Key::eK},
			{SDL_SCANCODE_L, se::window::Key::eL},
			{SDL_SCANCODE_M, se::window::Key::eM},
			{SDL_SCANCODE_N, se::window::Key::eN},
			{SDL_SCANCODE_O, se::window::Key::eO},
			{SDL_SCANCODE_P, se::window::Key::eP},
			{SDL_SCANCODE_Q, se::window::Key::eQ},
			{SDL_SCANCODE_R, se::window::Key::eR},
			{SDL_SCANCODE_S, se::window::Key::eS},
			{SDL_SCANCODE_T, se::window::Key::eT},
			{SDL_SCANCODE_U, se::window::Key::eU},
			{SDL_SCANCODE_V, se::window::Key::eV},
			{SDL_SCANCODE_W, se::window::Key::eW},
			{SDL_SCANCODE_X, se::window::Key::eX},
			{SDL_SCANCODE_Y, se::window::Key::eY},
			{SDL_SCANCODE_Z, se::window::Key::eZ},
			{SDL_SCANCODE_1, se::window::Key::e1},
			{SDL_SCANCODE_2, se::window::Key::e2},
			{SDL_SCANCODE_3, se::window::Key::e3},
			{SDL_SCANCODE_4, se::window::Key::e4},
			{SDL_SCANCODE_5, se::window::Key::e5},
			{SDL_SCANCODE_6, se::window::Key::e6},
			{SDL_SCANCODE_7, se::window::Key::e7},
			{SDL_SCANCODE_8, se::window::Key::e8},
			{SDL_SCANCODE_9, se::window::Key::e9},
			{SDL_SCANCODE_0, se::window::Key::e0},
			{SDL_SCANCODE_F1, se::window::Key::eF1},
			{SDL_SCANCODE_F2, se::window::Key::eF2},
			{SDL_SCANCODE_F3, se::window::Key::eF3},
			{SDL_SCANCODE_F4, se::window::Key::eF4},
			{SDL_SCANCODE_F5, se::window::Key::eF5},
			{SDL_SCANCODE_F6, se::window::Key::eF6},
			{SDL_SCANCODE_F7, se::window::Key::eF7},
			{SDL_SCANCODE_F8, se::window::Key::eF8},
			{SDL_SCANCODE_F9, se::window::Key::eF9},
			{SDL_SCANCODE_F10, se::window::Key::eF10},
			{SDL_SCANCODE_F11, se::window::Key::eF11},
			{SDL_SCANCODE_F12, se::window::Key::eF12},
			{SDL_SCANCODE_ESCAPE, se::window::Key::eEscape},
			{SDL_SCANCODE_TAB, se::window::Key::eTab},
			{SDL_SCANCODE_CAPSLOCK, se::window::Key::eCapslock},
			{SDL_SCANCODE_LSHIFT, se::window::Key::eLShift},
			{SDL_SCANCODE_LCTRL, se::window::Key::eLCtrl},
			{SDL_SCANCODE_LALT, se::window::Key::eLAlt},
			{SDL_SCANCODE_RALT, se::window::Key::eRAlt},
			{SDL_SCANCODE_LCTRL, se::window::Key::eLCtrl},
			{SDL_SCANCODE_RSHIFT, se::window::Key::eRShift},
			{SDL_SCANCODE_SPACE, se::window::Key::eSpace},
			{SDL_SCANCODE_RETURN, se::window::Key::eEnter},
			{SDL_SCANCODE_BACKSPACE, se::window::Key::eBackspace},
			{SDL_SCANCODE_DELETE, se::window::Key::eDelete},
			{SDL_SCANCODE_INSERT, se::window::Key::eInsert},
			{SDL_SCANCODE_HOME, se::window::Key::eHome},
			{SDL_SCANCODE_END, se::window::Key::eEnd},
			{SDL_SCANCODE_PAGEUP, se::window::Key::ePageUp},
			{SDL_SCANCODE_PAGEDOWN, se::window::Key::ePageDown},
			{SDL_SCANCODE_UP, se::window::Key::eUp},
			{SDL_SCANCODE_DOWN, se::window::Key::eDown},
			{SDL_SCANCODE_LEFT, se::window::Key::eLeft},
			{SDL_SCANCODE_RIGHT, se::window::Key::eRight}
		};

		auto it {keys.find(key)};
		SE_ASSERT(it != keys.end(), "Can't return SDL3 -> SE key if the mapping is not defined");
		return it->second;
	}


	se::window::MouseButton convertMouseButtonSDL3ToSe(se::Uint8 mouseButton) {
		static const std::unordered_multimap<se::Uint8, se::window::MouseButton> mouseButtons {
			{SDL_BUTTON_LEFT, se::window::MouseButton::eLeft},
			{SDL_BUTTON_MIDDLE, se::window::MouseButton::eMiddle},
			{SDL_BUTTON_RIGHT, se::window::MouseButton::eRight},
			{SDL_BUTTON_X1, se::window::MouseButton::eX1},
			{SDL_BUTTON_X2, se::window::MouseButton::eX2},
		};

		auto it {mouseButtons.find(mouseButton)};
		SE_ASSERT(it != mouseButtons.end(), "Can't return SDL3 -> SE mouse button if the mapping is not defined");
		return it->second;
	}
#endif
} // namespace se::window
