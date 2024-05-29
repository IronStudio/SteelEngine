#pragma once

#include <string>

#include "se/renderer/context.hpp"



namespace se::renderer {
	enum class ShaderType {
		eVertex,
		eFragment,
		eGeometry,
		eTessellation
	};

	struct ShaderInfos {
		se::renderer::Context *context;
		se::renderer::ShaderType type;
		std::string file;
		std::string entryPoint;
	};

	class Shader {

	};

} // namespace se::renderer
