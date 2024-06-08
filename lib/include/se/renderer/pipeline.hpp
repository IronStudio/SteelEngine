#pragma once

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/format.hpp"
#include "se/renderer/shader.hpp"
#include "se/renderer/uniformBufferView.hpp"
#include "se/renderer/vertexBufferView.hpp"



namespace se::renderer {
	enum class BlendMode {
		eNone,
		eSrcAlpha
	};

	enum class PipelineType {
		eRasterization,
		eRaytracing,
		eCompute
	};

	struct PipelineInfos {
		se::renderer::Context *context;
		se::renderer::VertexBufferView *vertexBufferView;
		std::vector<se::renderer::UniformBufferView*> uniformBufferView;
		std::vector<se::renderer::Shader*> shaders;
		std::vector<se::renderer::Format> colorAttachmentFormats;
		se::renderer::Format depthAttachmentFormat;
		se::renderer::Format stencilAttachmentFormat;
		se::renderer::BlendMode blendMode {se::renderer::BlendMode::eSrcAlpha};
		se::renderer::PipelineType type {se::renderer::PipelineType::eRasterization};
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
