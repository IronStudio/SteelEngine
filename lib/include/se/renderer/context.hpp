#pragma once

#include <string>

#include "se/renderer/gpuType.hpp"
#include "se/utils/version.hpp"



namespace se::renderer {
	struct ContextInfos {
		std::string applicationName;
		se::utils::Version applicationVersion;
		GPUType preferredGPU;
	};

	class Context {
		public:
			inline Context(const se::renderer::ContextInfos &infos) : m_infos {infos} {}
			virtual ~Context() = default;

		protected:
			se::renderer::ContextInfos m_infos;
	};

} // namespace se::renderer