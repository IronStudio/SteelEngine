#pragma once

#include <vector>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/shader.hpp"



namespace se::renderer {
	enum class UniformType {
		eFloat32,
		eInt32,
		eVec2,
		eVec3,
		eVec4,
		eMat2,
		eMat3,
		eMat4,
		eVec2i,
		eVec3i,
		eVec4i
	};

	struct UniformAttribute {
		std::string name;
		se::renderer::UniformType type;
	};

	struct UniformBufferViewInfos {
		se::renderer::Context *context;
		std::vector<se::renderer::UniformAttribute> attributes;
		se::ByteCount offset;
		se::renderer::ShaderTypeMask shaderTypes;
		se::Uint64 binding;
	};

	class SE_CORE UniformBufferView {
		public:
			inline UniformBufferView(const se::renderer::UniformBufferViewInfos &infos) : m_infos {infos} {}
			virtual ~UniformBufferView() = default;

			inline const se::renderer::UniformBufferViewInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::UniformBufferViewInfos m_infos;
	};

} // namespace se::renderer
