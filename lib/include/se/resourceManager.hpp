#pragma once

#include <functional>
#include <map>
#include <string>

#include "se/core.hpp"
#include "se/uuid.hpp"

#include "se/renderer/context.hpp"
#include "se/renderer/graphicsApi.hpp"



namespace se {
	template <typename T>
	struct Resource {
		se::UUID uuid {0};
		T *res {nullptr};

		inline ~Resource();
	};

	class SE_CORE ResourceManager {
		public:
			struct RendererContextInfos : public se::renderer::ContextInfos {
				se::renderer::GraphicsApi api;
			};

			static void load();
			static void unload();

			static se::Resource<se::renderer::Context> load(const RendererContextInfos &infos);

			static void unload(const se::Resource<se::renderer::Context> &context);

		private:
			template <typename T>
			using CreateFuncMap = std::map<se::renderer::GraphicsApi, std::function<T>>;

			static CreateFuncMap<se::renderer::Context*(const se::renderer::ContextInfos &)> s_rendererContextCreate;
	};


	template <typename T>
	Resource<T>::~Resource() {
		if (uuid != 0 && res != nullptr)
			se::ResourceManager::unload(*this);
	}

} // namespace se
