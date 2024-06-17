#include "se/renderer/renderer.hpp"



namespace se::renderer {

	Renderer::Renderer(const se::renderer::RendererInfos &infos) :
		m_infos {infos},
		m_context {},
		m_computeShader {}
	{
		se::ResourceManager::RendererContext contextInfos {};
		contextInfos.api = m_infos.api;
		contextInfos.applicationName = m_infos.application->getInfos().name;
		contextInfos.applicationVersion = m_infos.application->getInfos().version;
		contextInfos.linkedWindow = m_infos.window;
		contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
		m_context = std::move(se::ResourceManager::load(contextInfos));

		se::ResourceManager::RendererShader computeShaderInfos {};
		computeShaderInfos.context = m_context.res;
		computeShaderInfos.file = "test.comp";
		computeShaderInfos.entryPoint = "main";
		computeShaderInfos.type = se::renderer::ShaderType::eCompute;
	}


	Renderer::~Renderer() {
		se::ResourceManager::unload(m_context);
	}


	void Renderer::render() {

	}


} // namespace se::renderer
