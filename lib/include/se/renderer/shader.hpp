#pragma once

#include <string>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/utils/bitField.hpp"



namespace se::renderer {
	SE_CREATE_BIT_FIELD(ShaderType, ShaderTypeMask,
		eVertex   = 0b0000'0001,
		eFragment = 0b0000'0010,
		eGeometry = 0b0000'0100,
		eCompute  = 0b0000'1000
	);

	constexpr size_t SE_SHADER_TYPE_COUNT {3};

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

			inline const se::renderer::ShaderInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::ShaderInfos m_infos;
	};

} // namespace se::renderer
