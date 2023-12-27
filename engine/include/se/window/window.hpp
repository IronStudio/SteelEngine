#pragma once

#include <string>

#include "../graphicsApi.hpp"
#include "../math/vec2.hpp"



namespace se
{
	struct WindowInfos
	{
		std::string title;
		se::Vec2i size;
		bool resizable;
		bool fullscreen;
		se::GraphicsApi graphicsApi;
	};


	class Window
	{
		public:
			Window(const se::WindowInfos &infos);
			virtual ~Window() = default;

			inline const se::WindowInfos &getInfos() const noexcept;
			virtual void *getWindow() const noexcept = 0;

			virtual void toggleFullscreen() = 0;


		protected:
			se::WindowInfos m_infos;
	};



} // namespace se
