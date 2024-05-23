#pragma once

#include "se/renderer/context.hpp"



namespace se::renderer {
	enum class BufferUsage {
		eTransfertSrc,
		eTransfertDst,
		eUniform,
		eStorage,
		eIndex,
		eVertex,
		eIndirect
	};

	struct BufferInfos {
		se::renderer::Context *context;
		se::ByteCount size;
		se::renderer::BufferUsage usage;
	};

	class Buffer {
		public:
			inline Buffer(const se::renderer::BufferInfos &infos) : m_infos {infos} {}
			virtual ~Buffer() = default;

			inline const se::renderer::BufferInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::BufferInfos m_infos;
	};

} // namespace se::renderer
