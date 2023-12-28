#pragma once

#include <glm/glm.hpp>



namespace se
{
	enum class InputType
	{
		eKeyDown = 0, eKeyUp, eKeyPressed, eKeyReleased,
		eMouseButtonDown, eMouseButtonUp, eMouseButtonPressed, eMouseButtonReleased,
		eMouseMotion, eMouseScroll,
		eWindowMotion, eWindowResize, eWindowMaximized, eWindowMinimized, eWindowRestored,
		eWindowClosed,
		__last
	};



	enum class Key
	{
		eA = 0, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ,
		e1, e2, e3, e4, e5, e6, e7, e8, e9, e0,
		eF1, eF2, eF3, eF4, eF5, eF6, eF7, eF8, eF9, eF10, eF11, eF12,
		eSpace,
		eLCtrl, eRCtrl,
		eLAlt, eRAlt,
		eLShift, eRShift,
		eTab,
		eCapslock,
		eUp, eDown, eLeft, eRight,
		eEscape,
		eEnter,
		eDelete, eBackspace,
		eHome, eEnd, ePageUp, ePageDown,
		eKP1, eKP2, eKP3, eKP4, eKP5, eKP6, eKP7, eKP8, eKP9, eKP0
	};



	enum class MouseButton
	{
		eLeft = 0,
		eRight,
		eMiddle,
		eX1,
		eX2
	};



	struct MouseMotion
	{
		glm::vec2 position;
		glm::vec2 motion;
	};



	struct WindowMotion
	{
		glm::vec2 position;
		glm::vec2 motion;
	};



	struct WindowResize
	{
		glm::ivec2 oldSize;
		glm::ivec2 newSize;
	};



} // namespace se
