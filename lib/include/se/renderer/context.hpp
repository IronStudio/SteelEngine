#pragma once

#include <string>

#include "se/core.hpp"
#include "se/renderer/gpuType.hpp"
#include "se/utils/version.hpp"
#include "se/window/window.hpp"



namespace se::renderer {
	struct ContextInfos {
		std::string applicationName;
		se::utils::Version applicationVersion;
		GPUType preferredGPU;
		se::window::Window *linkedWindow;
	};

	class SE_CORE Context {
		public:
			inline Context(const se::renderer::ContextInfos &infos) : m_infos {infos} {}
			virtual ~Context() = default;

			virtual void handleWindowSizeChanged() = 0;
			virtual se::renderer::GraphicsApi getGraphicsApi() = 0;

		protected:
			se::renderer::ContextInfos m_infos;
	};

} // namespace se::renderer
