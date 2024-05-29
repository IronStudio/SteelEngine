#pragma once

#include "se/core.hpp"
#include "se/renderer/context.hpp"



namespace se::renderer {
	struct RenderPassInfos {
		se::renderer::Context *context;
	};

	class SE_CORE RenderPass {
		public:
			inline RenderPass(const se::renderer::RenderPassInfos &infos) : m_infos {infos} {}
			virtual ~RenderPass() = default;

			inline const se::renderer::RenderPassInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::RenderPassInfos m_infos;
	};

} // namespace se::renderer
