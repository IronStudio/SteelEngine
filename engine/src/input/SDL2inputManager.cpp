#include "input/SDL2inputManager.hpp"

#include "eventManager.hpp"
#include "logging.hpp"
#include "window/windowManager.hpp"



namespace se
{
	std::map<SDL_Keycode, se::Key> SDL2InputManager::s_SEKeyFromSDL2Key {
		{SDLK_a, se::Key::eA},
		{SDLK_b, se::Key::eB},
		{SDLK_c, se::Key::eC},
		{SDLK_d, se::Key::eD},
		{SDLK_e, se::Key::eE},
		{SDLK_f, se::Key::eF},
		{SDLK_g, se::Key::eG},
		{SDLK_h, se::Key::eH},
		{SDLK_i, se::Key::eI},
		{SDLK_j, se::Key::eJ},
		{SDLK_k, se::Key::eK},
		{SDLK_l, se::Key::eL},
		{SDLK_m, se::Key::eM},
		{SDLK_n, se::Key::eN},
		{SDLK_o, se::Key::eO},
		{SDLK_p, se::Key::eP},
		{SDLK_q, se::Key::eQ},
		{SDLK_r, se::Key::eR},
		{SDLK_s, se::Key::eS},
		{SDLK_t, se::Key::eT},
		{SDLK_u, se::Key::eU},
		{SDLK_v, se::Key::eV},
		{SDLK_w, se::Key::eW},
		{SDLK_x, se::Key::eX},
		{SDLK_y, se::Key::eY},
		{SDLK_z, se::Key::eZ},
		{SDLK_1, se::Key::e1},
		{SDLK_2, se::Key::e2},
		{SDLK_3, se::Key::e3},
		{SDLK_4, se::Key::e4},
		{SDLK_5, se::Key::e5},
		{SDLK_6, se::Key::e6},
		{SDLK_7, se::Key::e7},
		{SDLK_8, se::Key::e8},
		{SDLK_9, se::Key::e9},
		{SDLK_0, se::Key::e0},
		{SDLK_F1, se::Key::eF1},
		{SDLK_F2, se::Key::eF2},
		{SDLK_F3, se::Key::eF3},
		{SDLK_F4, se::Key::eF4},
		{SDLK_F5, se::Key::eF5},
		{SDLK_F6, se::Key::eF6},
		{SDLK_F7, se::Key::eF7},
		{SDLK_F8, se::Key::eF8},
		{SDLK_F9, se::Key::eF9},
		{SDLK_F10, se::Key::eF10},
		{SDLK_F11, se::Key::eF11},
		{SDLK_F12, se::Key::eF12},
		{SDLK_SPACE, se::Key::eSpace},
		{SDLK_LCTRL, se::Key::eLCtrl},
		{SDLK_RCTRL, se::Key::eRCtrl},
		{SDLK_LALT, se::Key::eLAlt},
		{SDLK_RALT, se::Key::eRAlt},
		{SDLK_LSHIFT, se::Key::eLShift},
		{SDLK_RSHIFT, se::Key::eRShift},
		{SDLK_TAB, se::Key::eTab},
		{SDLK_CAPSLOCK, se::Key::eCapslock},
		{SDLK_UP, se::Key::eUp},
		{SDLK_DOWN, se::Key::eDown},
		{SDLK_LEFT, se::Key::eLeft},
		{SDLK_RIGHT, se::Key::eRight},
		{SDLK_ESCAPE, se::Key::eEscape},
		{SDLK_RETURN, se::Key::eEnter},
		{SDLK_DELETE, se::Key::eDelete},
		{SDLK_BACKSPACE, se::Key::eBackspace},
		{SDLK_HOME, se::Key::eHome},
		{SDLK_END, se::Key::eEnd},
		{SDLK_PAGEUP, se::Key::ePageUp},
		{SDLK_PAGEDOWN, se::Key::ePageDown},
		{SDLK_KP_1, se::Key::eKP1},
		{SDLK_KP_2, se::Key::eKP2},
		{SDLK_KP_3, se::Key::eKP3},
		{SDLK_KP_4, se::Key::eKP4},
		{SDLK_KP_5, se::Key::eKP5},
		{SDLK_KP_6, se::Key::eKP6},
		{SDLK_KP_7, se::Key::eKP7},
		{SDLK_KP_8, se::Key::eKP8},
		{SDLK_KP_9, se::Key::eKP9},
		{SDLK_KP_0, se::Key::eKP0}
	};
	std::map<se::Uint8, se::MouseButton> SDL2InputManager::s_SEMouseButtonFromSDL2MouseButton {
		{SDL_BUTTON_LEFT, se::MouseButton::eLeft},
		{SDL_BUTTON_RIGHT, se::MouseButton::eRight},
		{SDL_BUTTON_MIDDLE, se::MouseButton::eMiddle},
		{SDL_BUTTON_X1, se::MouseButton::eX1},
		{SDL_BUTTON_X2, se::MouseButton::eX2}
	};



	SDL2InputManager::SDL2InputManager() :
		m_lastFramePressedKey {},
		m_lastFramePressedMouseButton {},
		m_lastFrameWindowSize {},
		m_lastFrameWindowPosition {},
		m_shouldClose {false}
	{
		for (se::Uint64 i {0}; i < (se::Uint64)se::Key::eKP0; ++i)
			m_lastFramePressedKey[(se::Key)i] = false;

		for (se::Uint64 i {0}; i < (se::Uint64)se::MouseButton::eX2; ++i)
			m_lastFramePressedMouseButton[(se::MouseButton)i] = false;

		for (const auto &window : se::WindowManager::getWindows())
		{
			m_lastFrameWindowSize[window->getUUID()] = window->getInfos().size;
			m_lastFrameWindowPosition[window->getUUID()] = window->getInfos().position;
		}
	}



	SDL2InputManager::~SDL2InputManager()
	{

	}



	se::Window *getWindowFromCode(se::Uint32 windowID)
	{
		void *windowPtr {SDL_GetWindowFromID(windowID)};

		for (const auto &window : se::WindowManager::getWindows())
		{
			if (window->getWindow() == windowPtr)
				return window;
		}

		throw std::invalid_argument("Can't find window with the given SDL2's id");
	}



	void SDL2InputManager::update()
	{
		static SDL_Event event {};

		std::map<se::Key, bool> keys {};
		std::map<se::MouseButton, bool> mouseButtons {};


		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_MOUSEMOTION:
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = getWindowFromCode(event.motion.windowID)->getEventTypes().find(se::InputType::eMouseMotion)->second;
					seevent.data = se::MouseMotion(
						glm::vec2(event.motion.x, event.motion.y),
						glm::vec2(event.motion.xrel, event.motion.yrel)
					);
					se::EventManager::notify(seevent);
					break;
				}


				case SDL_MOUSEBUTTONDOWN:
					mouseButtons[s_SEMouseButtonFromSDL2MouseButton[event.button.button]] = true;
					break;


				case SDL_MOUSEBUTTONUP:
					mouseButtons[s_SEMouseButtonFromSDL2MouseButton[event.button.button]] = false;
					break;


				case SDL_KEYDOWN:
					keys[s_SEKeyFromSDL2Key[event.key.keysym.sym]] = true;
					break;


				case SDL_KEYUP:
					keys[s_SEKeyFromSDL2Key[event.key.keysym.sym]] = false;
					break;

				
				case SDL_MOUSEWHEEL:
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = getWindowFromCode(event.wheel.windowID)->getEventTypes().find(se::InputType::eMouseScroll)->second;
					seevent.data = (float)event.wheel.preciseY;
					se::EventManager::notify(seevent);
					break;
				}

				case SDL_WINDOWEVENT:
				{
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eCanWait;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowMaximized)->second;
							seevent.data = se::WindowResize(m_lastFrameWindowSize[window->getUUID()],
								glm::ivec2(event.window.data1, event.window.data2)
							);
							se::EventManager::notify(seevent);
							break;
						}

						case SDL_WINDOWEVENT_MINIMIZED:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eCanWait;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowMinimized)->second;
							seevent.data = se::WindowResize(m_lastFrameWindowSize[window->getUUID()],
								glm::ivec2(event.window.data1, event.window.data2)
							);
							se::EventManager::notify(seevent);
							break;
						}

						case SDL_WINDOWEVENT_RESTORED:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eCanWait;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowRestored)->second;
							seevent.data = se::WindowResize(m_lastFrameWindowSize[window->getUUID()],
								glm::ivec2(event.window.data1, event.window.data2)
							);
							se::EventManager::notify(seevent);
							break;
						}

						case SDL_WINDOWEVENT_RESIZED:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eCanWait;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowResize)->second;
							seevent.data = se::WindowResize(m_lastFrameWindowSize[window->getUUID()],
								glm::ivec2(event.window.data1, event.window.data2)
							);
							se::EventManager::notify(seevent);
							break;
						}

						case SDL_WINDOWEVENT_MOVED:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eCanWait;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowMotion)->second;
							seevent.data = se::WindowMotion(glm::ivec2(event.window.data1, event.window.data2),
								glm::ivec2(event.window.data1, event.window.data2) - m_lastFrameWindowPosition[window->getUUID()]
							);
							se::EventManager::notify(seevent);
							break;
						}

						case SDL_WINDOWEVENT_CLOSE:
						{
							se::Window *window {getWindowFromCode(event.window.windowID)};
							se::Event seevent {};
							seevent.priority = se::EventPriority::eBlocking;
							seevent.type = window->getEventTypes().find(se::InputType::eWindowClosed)->second;
							se::EventManager::notify(seevent);
							break;
						}
					}

					break;
				}

				case SDL_QUIT:
					m_shouldClose = true;
					break;
			}
		}



		void *SDLKeyboardWindow {SDL_GetKeyboardFocus()};
		se::Window *keyboardWindow {nullptr};
		if (SDLKeyboardWindow != nullptr)
		{
			for (const auto &window : se::WindowManager::getWindows())
			{
				if (window->getWindow() != SDLKeyboardWindow)
					continue;

				keyboardWindow = window;
				break;
			}

			if (keyboardWindow == nullptr)
				throw std::runtime_error("Can't find window with keyboard's focus");
		}

		void *SDLMouseWindow {SDL_GetMouseFocus()};
		se::Window *mouseWindow {nullptr};

		if (SDLMouseWindow != nullptr)
		{
			for (const auto &window : se::WindowManager::getWindows())
			{
				if (window->getWindow() != SDLMouseWindow)
					continue;

				mouseWindow = window;
				break;
			}

			if (mouseWindow == nullptr)
				throw std::runtime_error("Can't find window with mouse's focus");
		}

		if (keyboardWindow != nullptr)
		{
			for (const auto &key : keys)
			{
				if (key.second)
				{
					if (!m_lastFramePressedKey[key.first])
					{
						se::Event seevent {};
						seevent.priority = se::EventPriority::eCanWait;
						seevent.type = keyboardWindow->getEventTypes().find(se::InputType::eKeyPressed)->second;
						seevent.data = key.first;
						se::EventManager::notify(seevent);
					}
				}

				else
				{
					if (m_lastFramePressedKey[key.first])
					{
						se::Event seevent {};
						seevent.priority = se::EventPriority::eCanWait;
						seevent.type = keyboardWindow->getEventTypes().find(se::InputType::eKeyReleased)->second;
						seevent.data = key.first;
						se::EventManager::notify(seevent);
					}
				}

				m_lastFramePressedKey[key.first] = key.second;
			}

			for (const auto &key: m_lastFramePressedKey)
			{
				if (key.second)
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = keyboardWindow->getEventTypes().find(se::InputType::eKeyDown)->second;
					seevent.data = key.first;
					se::EventManager::notify(seevent);
				}

				else
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = keyboardWindow->getEventTypes().find(se::InputType::eKeyUp)->second;
					seevent.data = key.first;
					se::EventManager::notify(seevent);
				}
			}
		}



		if (mouseWindow != nullptr)
		{
			for (const auto &button : mouseButtons)
			{
				if (button.second)
				{
					if (!m_lastFramePressedMouseButton[button.first])
					{
						se::Event seevent {};
						seevent.priority = se::EventPriority::eCanWait;
						seevent.type = mouseWindow->getEventTypes().find(se::InputType::eMouseButtonPressed)->second;
						seevent.data = button.first;
						se::EventManager::notify(seevent);
					}
				}

				else
				{
					if (m_lastFramePressedMouseButton[button.first])
					{
						se::Event seevent {};
						seevent.priority = se::EventPriority::eCanWait;
						seevent.type = mouseWindow->getEventTypes().find(se::InputType::eMouseButtonReleased)->second;
						seevent.data = button.first;
						se::EventManager::notify(seevent);
					}
				}

				m_lastFramePressedMouseButton[button.first] = button.second;
			}

			for (const auto &button: m_lastFramePressedMouseButton)
			{
				if (button.second)
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = mouseWindow->getEventTypes().find(se::InputType::eMouseButtonDown)->second;
					seevent.data = button.first;
					se::EventManager::notify(seevent);
				}

				else
				{
					se::Event seevent {};
					seevent.priority = se::EventPriority::eCanWait;
					seevent.type = mouseWindow->getEventTypes().find(se::InputType::eMouseButtonUp)->second;
					seevent.data = button.first;
					se::EventManager::notify(seevent);
				}
			}
		}


		for (const auto &window : se::WindowManager::getWindows())
		{
			m_lastFrameWindowSize[window->getUUID()] = window->getInfos().size;
			m_lastFrameWindowPosition[window->getUUID()] = window->getInfos().position;
		}
	}



	bool SDL2InputManager::shouldClose()
	{
		return m_shouldClose;
	}



} // namespace se
