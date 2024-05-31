#pragma once

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/shader.hpp"
#include "se/renderer/vertexBufferView.hpp"



namespace se::renderer {
	struct PipelineInfos {
		se::renderer::Context *context;
		se::renderer::VertexBufferView *vertexBufferView;
		std::vector<se::renderer::Shader*> shaders;
	};

	class SE_CORE Pipeline {
		public:
			inline Pipeline(const se::renderer::PipelineInfos &infos) : m_infos {infos} {}
			virtual ~Pipeline() = default;

			inline const se::renderer::PipelineInfos &getInfos() const noexcept {return m_infos;}


		protected:
			se::renderer::PipelineInfos m_infos;
	};

} // namespace se::renderer
