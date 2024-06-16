#include "se/resourceManager.hpp"

#include "se/logger.hpp"

#include "se/renderer/opengl/context.hpp"

#include "se/renderer/vulkan/context.hpp"



namespace se {

	void ResourceManager::load() {

	}


	void ResourceManager::unload() {

	}


	se::Resource<se::renderer::Context> ResourceManager::load(const RendererContextInfos &infos) {
		se::Resource<se::renderer::Context> resource {};
		resource.uuid = {};
		resource.res = s_rendererContextCreate[infos.api](infos);
		return resource;
	}


	void ResourceManager::unload(const se::Resource<se::renderer::Context> &context) {
		delete context.res;
	}


	#define STRAIGHT_REPLACEMENT(api, Infos, Type) {se::renderer::GraphicsApi::api, [](const se::renderer::Infos &infos) {\
		return new se::renderer::Type(infos);\
	}}

	ResourceManager::CreateFuncMap<se::renderer::Context*(const se::renderer::ContextInfos &)> ResourceManager::s_rendererContextCreate {
		STRAIGHT_REPLACEMENT(eVulkan, ContextInfos, vulkan::Context),
		STRAIGHT_REPLACEMENT(eOpenGL, ContextInfos, opengl::Context),
	};

} // namespace se
