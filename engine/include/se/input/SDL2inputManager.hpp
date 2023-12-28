#pragma once

#include <map>

#include <SDL2/SDL.h>

#include "../core.hpp"
#include "../uuid.hpp"
#include "inputManager.hpp"
#include "type.hpp"



namespace se
{
	class SE_CORE SDL2InputManager final : public se::InputManager
	{
		public:
			SDL2InputManager();
			~SDL2InputManager() override;

			void update() override;
			bool shouldClose() override;


		private:
			static std::map<SDL_Keycode, se::Key> s_SEKeyFromSDL2Key;
			static std::map<se::Uint8, se::MouseButton> s_SEMouseButtonFromSDL2MouseButton;

			std::map<se::Key, bool> m_lastFramePressedKey;
			std::map<se::MouseButton, bool> m_lastFramePressedMouseButton;
			std::map<se::UUID, glm::ivec2> m_lastFrameWindowSize;
			std::map<se::UUID, glm::ivec2> m_lastFrameWindowPosition;
			bool m_shouldClose;
	};



} // namespace se
