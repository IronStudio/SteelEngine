#pragma once

#include <vector>

#include "se/renderer/context.hpp"



namespace se::renderer {
	enum class VertexType {
		eFloat32,
		eInt32,
		eUint32
	};

	struct VertexAttribute {
		se::renderer::VertexType type;
		se::Uint64 location;
		se::Uint64 dimension;
		se::Uint64 binding;
	};

	struct VertexBufferViewInfos {
		se::renderer::Context *context;
		std::vector<se::renderer::VertexAttribute> attributes;
	};

	class VertexBufferView {
		public:
			inline VertexBufferView(const se::renderer::VertexBufferViewInfos &infos) : m_infos {infos} {}
			virtual ~VertexBufferView() = default;

			inline const se::renderer::VertexBufferViewInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::VertexBufferViewInfos m_infos;
	};

} // namespace se::renderer
