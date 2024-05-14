#pragma once

#include <unordered_map>

#include "se/concepts.hpp"
#include "se/core.hpp"


namespace se::window {
	enum class Key {
		eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ,
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
		eUp, eDown, eLeft, eRight
	};


	enum class MouseButton {
		eLeft,
		eMiddle,
		eRight,
		eX1,
		eX2
	};
} // namespace se::window


#ifndef SE_APPLE
namespace se::concepts {
	template <typename T>
	concept InputManager = requires(T obj) {
		{obj.update()} -> std::same_as<void>;
		{obj.isKeyDown(se::window::Key::eA)} -> std::same_as<bool>;
		{obj.isKeyUp(se::window::Key::eA)} -> std::same_as<bool>;
		{obj.wasKeyPressed(se::window::Key::eA)} -> std::same_as<bool>;
		{obj.wasKeyReleased(se::window::Key::eA)} -> std::same_as<bool>;
		{obj.isMouseButtonDown(se::window::MouseButton::eLeft)} -> std::same_as<bool>;
		{obj.isMouseButtonUp(se::window::MouseButton::eLeft)} -> std::same_as<bool>;
		{obj.wasMouseButtonPressed(se::window::MouseButton::eLeft)} -> std::same_as<bool>;
		{obj.wasMouseButtonReleased(se::window::MouseButton::eLeft)} -> std::same_as<bool>;
	};
} // namespace se::concepts
#endif



namespace se::window {
	template <typename Implementation>
	SE_REQUIRES(se::concepts::InputManager<Implementation>)
	class InputManagerBase final {
		public:
			inline static void update();
			inline static bool isKeyDown(se::window::Key key);
			inline static bool isKeyUp(se::window::Key key);
			inline static bool wasKeyPressed(se::window::Key key);
			inline static bool wasKeyReleased(se::window::Key key);

			inline static bool isMouseButtonDown(se::window::MouseButton mouseButton);
			inline static bool isMouseButtonUp(se::window::MouseButton mouseButton);
			inline static bool wasMouseButtonPressed(se::window::MouseButton mouseButton);
			inline static bool wasMouseButtonReleased(se::window::MouseButton mouseButton);

		private:
			static Implementation s_implementation;
	};


#ifdef SE_WSI_SDL3
	
	class SE_CORE SDL3InputManager final {
		public:
			SDL3InputManager();
			~SDL3InputManager() = default;

			void update();

			bool isKeyDown(se::window::Key key) const noexcept;
			bool isKeyUp(se::window::Key key) const noexcept;
			bool wasKeyPressed(se::window::Key key) const noexcept;
			bool wasKeyReleased(se::window::Key key) const noexcept;

			bool isMouseButtonDown(se::window::MouseButton mouseButton) const noexcept;
			bool isMouseButtonUp(se::window::MouseButton mouseButton) const noexcept;
			bool wasMouseButtonPressed(se::window::MouseButton mouseButton) const noexcept;
			bool wasMouseButtonReleased(se::window::MouseButton mouseButton) const noexcept;

		private:
			std::unordered_map<se::window::Key, bool> m_areKeysDown;
			std::unordered_map<se::window::Key, bool> m_lastAreKeysDown;
			std::unordered_map<se::window::MouseButton, bool> m_areMouseButtonsDown;
			std::unordered_map<se::window::MouseButton, bool> m_lastAreMouseButtonsDown;
	};

	using InputManager = InputManagerBase<SDL3InputManager>;

#endif

} // namespace se::window



#include "se/window/input.inl"