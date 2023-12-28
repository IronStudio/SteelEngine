#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "../core.hpp"
#include "../graphicsApi.hpp"
#include "../input/type.hpp"
#include "../uuid.hpp"



namespace se
{
	#define SE_UNDEFINED_SIZE glm::ivec2(-1, -1)
	#define SE_CENTERED_POSITION std::numeric_limits<int>::max()


	struct WindowInfos
	{
		std::string title;
		glm::ivec2 size;
		glm::ivec2 minSize {SE_UNDEFINED_SIZE};
		glm::ivec2 maxSize {SE_UNDEFINED_SIZE};
		glm::ivec2 windowedSize {SE_UNDEFINED_SIZE};
		glm::ivec2 position {SE_CENTERED_POSITION, SE_CENTERED_POSITION};
		bool resizable {true};
		bool fullscreen {false};
		bool borderless {false};
		bool alwaysOnTop {false};
		bool minimized {false};
		bool maximized {false};
		float opacity {1.f};
		se::GraphicsApi graphicsApi;
	};


	class SE_CORE Window
	{
		public:
			Window(const se::WindowInfos &infos);
			virtual ~Window();

			inline se::UUID getUUID() const noexcept;
			inline const se::WindowInfos &getInfos() const noexcept;
			inline const std::map<se::InputType, se::UUID> &getEventTypes() const noexcept;
			virtual void *getWindow() const noexcept = 0;

			virtual void toggleFullscreen() = 0;
			virtual void setSize(const glm::ivec2 &size) = 0;
			virtual void setMinSize(const glm::ivec2 &size) = 0;
			virtual void setMaxSize(const glm::ivec2 &size) = 0;
			virtual void toggleResizablity() = 0;
			virtual void toggleBorder() = 0;
			virtual void toggleAlwaysOnTop() = 0;
			virtual void setTitle(const std::string &title) = 0;
			virtual void setOpacity(float opacity) = 0;
			virtual void setPosition(const glm::ivec2 &position) = 0;
			virtual void toggleMinimization() = 0;
			virtual void toggleMaximization() = 0;

			virtual void updateInfos() = 0;


		protected:
			se::UUID m_uuid;
			se::WindowInfos m_infos;
			std::map<se::InputType, se::UUID> m_eventTypes;
	};



} // namespace se



#include "window.inl"