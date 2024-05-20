#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "se/core.hpp"
#include "se/math.hpp"
#include "se/renderer/graphicsApi.hpp"
#include "se/uuid.hpp"



namespace se::window {
	constexpr se::Int centerPosition {std::numeric_limits<se::Int>::max()};
	constexpr se::Int undefinedPosition {std::numeric_limits<se::Int>::min()};

	struct WindowInfos {
		std::string title;
		se::Vec2i size;
		se::Vec2i position {se::window::centerPosition, se::window::centerPosition};
		se::renderer::GraphicsApi graphicsApi;
	};

	class SE_CORE Window {
		public:
			Window();
			~Window();

			Window(const se::window::WindowInfos &infos);

			Window(const se::window::Window &) = delete;
			const se::window::Window &operator=(const se::window::Window &) = delete;

			Window(se::window::Window &&window) noexcept;
			const se::window::Window &operator=(se::window::Window &&window) noexcept;


			void resize(const se::Vec2i &size);
			void move(se::Vec2i position);
			void setTitle(const std::string &title);

			inline const se::window::WindowInfos &getInfos() const noexcept;
			inline SDL_Window *getInternalObject() const noexcept;
			inline se::UUID getUUID() const noexcept;


		private:
			se::window::WindowInfos m_infos;
			SDL_Window *m_window;
			se::UUID m_uuid;
	};

} // namespace se::window



#include "se/window/window.inl"