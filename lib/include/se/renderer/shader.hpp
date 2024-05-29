#pragma once

#include <string>

#include "se/core.hpp"
#include "se/renderer/context.hpp"



namespace se::renderer {
	enum class ShaderType {
		eVertex,
		eFragment,
		eGeometry
	};

	struct ShaderInfos {
		se::renderer::Context *context;
		se::renderer::ShaderType type;
		std::string file;
		std::string entryPoint;
	};

	class SE_CORE Shader {
		public:
			inline Shader(const se::renderer::ShaderInfos &infos) : m_infos {infos} {}
			virtual ~Shader() = default;

		protected:
			se::renderer::ShaderInfos m_infos;
	};

} // namespace se::renderer
