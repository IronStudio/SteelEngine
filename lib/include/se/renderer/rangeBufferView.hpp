#pragma once

#include <map>
#include <vector>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/shader.hpp"
#include "se/renderer/bufferViewUsage.hpp"



namespace se::renderer {
	struct RangeBufferViewInfos {
		se::renderer::Context *context;
		se::ByteCount offset;
		se::renderer::ShaderTypeMask shaderTypes;
		se::Uint64 binding;
		se::ByteCount size;
		se::renderer::BufferViewUsage usage;
	};

	class SE_CORE RangeBufferView {
		public:
			inline RangeBufferView(const se::renderer::RangeBufferViewInfos &infos) : m_infos {infos} {}
			virtual ~RangeBufferView() = default;

			inline const se::renderer::RangeBufferViewInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::RangeBufferViewInfos m_infos;
	};

} // namespace se::renderer
