#pragma once

#include <map>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "se/core.hpp"
#include "se/math.hpp"
#include "se/uuid.hpp"



namespace se::input {
	enum class Key {
		eA = 0, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ,
		e1, e2, e3, e4, e5, e6, e7, e8, e9, e0,
		eF1, eF2, eF3, eF4, eF5, eF6, eF7, eF8, eF9, eF10, eF11, eF12,
		eEscape,
		eTab,
		eCapslock,
		eLShift, eLCtrl, eLAlt, eRAlt, eRCtrl, eRShift,
		eSpace,
		eEnter,
		eBackspace,
		eDelete,
		eInsert,
		eHome, eEnd, ePageUp, ePageDown,
		eUp, eDown, eLeft, eRight,

		__last
	};

	enum class MouseButton {
		eLeft = 0,
		eMiddle,
		eRight,
		eX1,
		eX2,

		__last
	};



	class SE_CORE InputManager {
		public:
			static void update();

			static bool isKeyDown(se::input::Key key);
			static bool isKeyUp(se::input::Key key);
			static bool wasKeyPressed(se::input::Key key);
			static bool wasKeyReleased(se::input::Key key);

			static bool isMouseButtonDown(se::input::MouseButton button);
			static bool isMouseButtonUp(se::input::MouseButton button);
			static bool wasMouseButtonPressed(se::input::MouseButton button);
			static bool wasMouseButtonReleased(se::input::MouseButton button);

			static se::UUID getFocusedWindowUUID();
			static bool wasWindowResized(se::UUID uuid);

			static const se::Vec2i &getMousePosition();
			static const se::Vec2i &getMouseMotion();
			static bool hasMouseMoved();


		private:
			static void s_load();
			static void s_handleWindowEvent(const SDL_Event &event);

			static std::unordered_map<se::input::Key, bool> s_keyStates;
			static std::unordered_map<se::input::Key, bool> s_oldKeyStates;
			static std::unordered_map<se::input::MouseButton, bool> s_mouseButtonStates;
			static std::unordered_map<se::input::MouseButton, bool> s_oldMouseButtonStates;
			static se::UUID s_focusedWindowUUID;
			static std::map<size_t, bool> s_wasWindowResized;
			static se::Vec2i s_mousePosition;
			static se::Vec2i s_mouseMotion;
	};

} // namespace se::input
