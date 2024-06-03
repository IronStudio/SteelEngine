#pragma once

#include "se/core.hpp"
#include "se/math.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/format.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se::renderer {
	struct DepthBufferInfos {
		se::renderer::Context *context;
		se::renderer::VramAllocator *allocator;
		se::Vec2i size;
		se::renderer::Format format;
	};

	class SE_CORE DepthBuffer {
		public:
			inline DepthBuffer(const se::renderer::DepthBufferInfos &infos) : m_infos {infos} {}
			virtual ~DepthBuffer() = default;

			inline const se::renderer::DepthBufferInfos &getInfos() const noexcept {return m_infos;}


		protected:
			se::renderer::DepthBufferInfos m_infos;
	};

} // namespace se::renderer
