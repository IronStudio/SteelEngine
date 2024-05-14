#pragma once

#include <string>

#ifdef SE_WSI_SDL3
	#include <SDL3/SDL.h>
#endif

#include "se/concepts.hpp"
#include "se/core.hpp"
#include "se/renderer/graphicsApi.hpp"
#include "se/types.hpp"



namespace se::window {
	struct WindowInfos {
		se::Int width;
		se::Int height;
		std::string title;
		se::renderer::GraphicsApi graphicsApi;
	};
}


#ifndef SE_APPLE
namespace se::concepts {
	template <typename T>
	concept WindowImplementation = requires(T obj) {
		T();
		{obj.create(se::window::WindowInfos())} -> std::same_as<void>;
		{obj.destroy()} -> std::same_as<void>;
		{obj.move(T())} -> std::same_as<void>;
	};
}
#endif


namespace se::window {
	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	class WindowBase {
		public:
			inline WindowBase();
			inline ~WindowBase();

			inline WindowBase(const se::window::WindowInfos &infos);

			inline WindowBase(const se::window::WindowBase<Implementation> &) = delete;
			inline const se::window::WindowBase<Implementation> &operator=(const se::window::WindowBase<Implementation> &) = delete;

			inline WindowBase(se::window::WindowBase<Implementation> &&window) noexcept;
			inline const se::window::WindowBase<Implementation> &operator=(se::window::WindowBase<Implementation> &&window) noexcept;


		private:
			Implementation m_implementation;
	};



#ifdef SE_WSI_SDL3

	class SE_CORE SDL3Window {
		public:
			SDL3Window();
			~SDL3Window();

			void create(const se::window::WindowInfos &infos);
			void destroy();
			void move(SDL3Window &&window);

		private:
			SDL_Window *m_window;
	};


	using Window = WindowBase<SDL3Window>;

#endif

} // namespace se::window


#include "se/window/window.inl"