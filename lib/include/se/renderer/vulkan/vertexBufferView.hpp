#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/vertexBufferView.hpp"



namespace se::renderer::vulkan {
	class VertexBufferView : public se::renderer::VertexBufferView {
		public:
			VertexBufferView(const se::renderer::VertexBufferViewInfos &infos);
			~VertexBufferView() override;

			inline const std::vector<VkVertexInputAttributeDescription> &getAttributeDescriptions() const noexcept {return m_attributeDescriptions;}
			inline const std::vector<VkVertexInputBindingDescription> &getBindingDescriptions() const noexcept {return m_bindingDescriptions;}

		private:
			std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
			std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
	};

} // namespace se::renderer::vulkan
