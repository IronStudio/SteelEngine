#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/attributeBufferView.hpp"



namespace se::renderer::vulkan {
	VkDescriptorType attributeBufferUsageSeToVk(se::renderer::AttributeBufferViewUsage usage);
	se::renderer::AttributeBufferViewUsage attributeBufferUsageVkToSe(VkDescriptorType usage);

	class SE_CORE AttributeBufferView final : public se::renderer::AttributeBufferView {
		public:
			AttributeBufferView(const se::renderer::AttributeBufferViewInfos &infos);
			~AttributeBufferView() override;

			inline const VkDescriptorSetLayoutBinding &getLayoutBinding() const noexcept {return m_layoutBinding;}

		private:
			VkDescriptorSetLayoutBinding m_layoutBinding;
	};

} // namespace se::renderer::vulkan
