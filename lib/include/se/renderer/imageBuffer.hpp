#pragma once

#include "se/core.hpp"
#include "se/math.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/format.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se::renderer {
	enum class ImageBufferUsage {
		eColorAttachment,
		eDepthAttachment
	};

	struct ImageBufferInfos {
		se::renderer::Context *context;
		se::renderer::VramAllocator *allocator;
		se::Vec2i size;
		se::renderer::Format format;
		se::renderer::ImageBufferUsage usage;
	};

	class SE_CORE ImageBuffer {
		public:
			inline ImageBuffer(const se::renderer::ImageBufferInfos &infos) : m_infos {infos} {}
			virtual ~ImageBuffer() = default;

			inline const se::renderer::ImageBufferInfos &getInfos() const noexcept {return m_infos;}


		protected:
			se::renderer::ImageBufferInfos m_infos;
	};

} // namespace se::renderer
